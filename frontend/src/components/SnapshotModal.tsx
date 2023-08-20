import { useEffect, useState } from "react";
import { Button, Form, Modal, ModalProps, Select } from "react-daisyui";
import { toast } from "react-toastify";
import { branches } from "../utils/SectionUtils";

export type SnapshotModalProps = ModalProps & {
  sections: ISection[];
  sourceBranch?: string;
  targetBranch?: string;
};

export default (props: SnapshotModalProps) => {
  const [sourceBranch, setSourceBranch] = useState(props.sourceBranch);

  useEffect(() => {
    setSourceBranch(props.sourceBranch);
  }, [props.sourceBranch]);

  const [targetBranch, setTargetBranch] = useState(props.targetBranch);

  useEffect(() => {
    setTargetBranch(props.targetBranch);
  }, [props.targetBranch]);

  return (
    <Modal {...props}>
      <h1 className="text-3xl font-bold">Snapshot</h1>
      <Form>
        <Form.Label title="Source branch" />
        <Select
          value={sourceBranch}
          size="sm"
          onChange={(event) => {
            setSourceBranch(event.target.value);
          }}
        >
          {branches(props.sections || []).map((value) => (
            <option>{value}</option>
          ))}
        </Select>
      </Form>

      <Form>
        <Form.Label title="Target branch" />
        <Select
          value={targetBranch}
          size="sm"
          onChange={(event) => {
            setTargetBranch(event.target.value);
          }}
        >
          {branches(props.sections || []).map((value) => (
            <option>{value}</option>
          ))}
        </Select>
      </Form>
      <Form className="mt-6">
        <span className="flex space-x-4 justify-end">
          <Button
            type="button"
            color="primary"
            onClick={(e) => {
              toast.success(`Snapped to ${targetBranch}`);
            }}
          >
            Snap
          </Button>
        </span>
      </Form>
    </Modal>
  );
};
