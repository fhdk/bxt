import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import {
  Button,
  Card,
  Form,
  Hero,
  Input,
  Link,
  Modal,
  ModalProps,
  Select,
  Table,
} from "react-daisyui";
import {
  faCube,
  faSignature,
  faTrashCan,
} from "@fortawesome/free-solid-svg-icons";
import packages from "../hooks/packages.json";
import {
  architecturesForBranchAndRepo,
  branches,
  reposForBranch,
} from "../utils/SectionUtils";
import { ToastContainer, toast } from "react-toastify";
import "react-toastify/dist/ReactToastify.css";
import * as uuid from "uuid";

import { useCallback, useEffect, useState } from "react";

export type CommitModalProps = ModalProps & {
  isNew?: boolean;
  commit?: ICommit;
  sections?: ISection[];
  onCommitSubmit?: (commit: ICommit) => void;
  onCommitDelete?: (id: string) => void;
};

export default (props: CommitModalProps) => {
  const [commit, setCommit] = useState<ICommit>(
    props.commit || { id: uuid.v4(), section: {}, packages: [] }
  );

  useEffect(
    () =>
      setCommit(props.commit || { id: uuid.v4(), section: {}, packages: [] }),
    [props.commit]
  );

  const { id, section, packages } = commit;

  const { branch, repository, architecture } = section;

  const setBranch = (branch: string) => {
    setCommit({ id, section: { ...section, branch }, packages });
  };

  const setRepository = (repository: string) => {
    setCommit({ id, section: { ...section, repository }, packages });
  };

  const setArchitecture = (architecture: string) => {
    setCommit({ id, section: { ...section, architecture }, packages });
  };

  const setPackages = (pkg: IPackageUpload) => {
    setCommit({ ...commit, packages: [...packages, pkg] });
  };

  const commitCallback = useCallback(() => {
    toast.success("Commited!");

    if (props.onCommitSubmit) props.onCommitSubmit(commit);
  }, [commit, props.onSubmit]);

  return (
    <Modal className="w-11/12 max-w-5xl" {...props}>
      <h1 className="text-3xl font-bold">Commit</h1>
      <Modal.Body>
        <Form>
          <Form.Label title="Branch" />
          <Select
            value={branch}
            size="sm"
            onChange={(event) => {
              setBranch(event.target.value);
            }}
          >
            {branches(props.sections || []).map((value) => (
              <option>{value}</option>
            ))}
          </Select>
        </Form>
        <Form>
          <Form.Label title="Repository" />
          <Select
            value={repository}
            size="sm"
            onChange={(event) => {
              setRepository(event.target.value);
            }}
          >
            {reposForBranch(props.sections || [], branch || "").map((value) => (
              <option>{value}</option>
            ))}
          </Select>
        </Form>
        <Form>
          <Form.Label title="Architecture" />
          <Select
            value={architecture}
            size="sm"
            onChange={(event) => {
              setArchitecture(event.target.value);
            }}
          >
            {architecturesForBranchAndRepo(
              props.sections || [],
              branch || "",
              repository || ""
            ).map((value) => (
              <option>{value}</option>
            ))}
          </Select>
        </Form>
        <Form>
          <Form.Label title="Packages" />

          <Table zebra={true} compact={true} className="overflow-x-auto">
            <Table.Head>
              <span>Name</span>
              <span />
            </Table.Head>
            <Table.Body>
              {props.commit?.packages.map((value) => {
                return (
                  <Table.Row>
                    <span className="flex items-center">
                      <FontAwesomeIcon icon={faCube} className="px-2 max-h-6" />
                      {value.hasSignature && (
                        <FontAwesomeIcon
                          icon={faSignature}
                          color="green"
                          className="px-2 max-h-6"
                        />
                      )}

                      {value.name}
                    </span>
                    <span></span>
                  </Table.Row>
                );
              })}
            </Table.Body>
          </Table>
        </Form>
      </Modal.Body>

      <Form className="mt-6">
        <span className="flex space-x-4 justify-end">
          {props.isNew && (
            <Button
              startIcon={<FontAwesomeIcon className="px-1" icon={faTrashCan} />}
              onClick={(e) => {
                if (props.onCommitDelete) props.onCommitDelete(commit.id);
              }}
              type="button"
              color="error"
            >
              Delete
            </Button>
          )}
          <Button onClick={commitCallback} type="button" color="primary">
            Commit
          </Button>
        </span>
      </Form>
    </Modal>
  );
};
