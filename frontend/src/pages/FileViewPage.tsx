import {
  FullFileBrowser,
  FileArray,
  setChonkyDefaults,
  ChonkyFileActionData,
  ChonkyActions,
  FileHelper,
} from "chonky";
import { ChonkyIconFA } from "chonky-icon-fontawesome";
import { useCallback, useEffect, useState } from "react";
import { IUpdateSections, useSections } from "../hooks/BxtHooks";
import Dropzone from "react-dropzone";
import CommitModal from "../components/CommitModal";
import { Button, Drawer, Menu } from "react-daisyui";
import CommitCard from "../components/CommitCard";
import { IUpdateFiles, useFilesFromSections } from "../hooks/BxtFsHooks";
import * as uuid from "uuid";
import { toast } from "react-toastify";
import SnapshotModal from "../components/SnapshotModal";
import {
  SnapshotAction,
  SnapshotActionPayload,
  SnapToAction,
} from "../components/SnapshotAction";
import { OpenFilesPayload } from "chonky/dist/types/action-payloads.types";
import axios from "axios";

setChonkyDefaults({ iconComponent: ChonkyIconFA });

const useFolderChainForPath = (path: string[]): FileArray => {
  const result: FileArray = [];

  result.push({
    id: "root",
    name: "root",
    isDir: true,
  });

  for (let i = 1; i < path.length; i++) {
    result.push({
      id: `${result[i - 1]!.id}/${path[i]}`,
      name: path[i],
      isDir: true,
    });
  }

  return result;
};

export const useFileActionHandler = (
  setPath: (path: string[]) => void,
  setSnapshotModalBranches: (
    sourceBranch?: string,
    targetBranch?: string
  ) => void
) => {
  return useCallback(
    (data: ChonkyFileActionData) => {
      switch (data.id as string) {
        case ChonkyActions.OpenFiles.id:
          const { targetFile, files } = data.payload as OpenFilesPayload;
          const fileToOpen = targetFile ?? files[0];
          if (fileToOpen && FileHelper.isDirectory(fileToOpen)) {
            const pathToOpen = fileToOpen.id.split("/");

            setPath(pathToOpen);
          }
          break;
        case "snap":
          const { sourceBranch, targetBranch } =
            data.payload as SnapshotActionPayload;

          setSnapshotModalBranches(sourceBranch, targetBranch);
      }
    },
    [setPath, setSnapshotModalBranches]
  );
};

const usePackageDropHandler = (
  path: string[],
  setCommit: (commits: ICommit) => void
) => {
  return useCallback(
    (acceptedFiles: File[]) => {
      const section: ISection = {
        branch: path[1],
        repository: path[2],
        architecture: path[3],
      };

      const packages = acceptedFiles.map((value) => {
        return { name: value.name, section, file: value };
      });

      setCommit({ id: uuid.v4(), section, packages });
    },
    [path, setCommit]
  );
};

const usePushCommitsHandler = (commits: ICommit[], reload: () => void) => {
  return useCallback(
    async (e: any) => {
      let formData = new FormData();

      let packages = commits.flatMap(value => value.packages);
      packages.forEach((pkg, index) => {
        if (pkg.file) {
          formData.append(`package${index + 1}.filepath`, pkg.file);
        }
        if (pkg.hasSignature) {
          formData.append(`package${index + 1}.signature_path`, `${pkg.file}.sig`);
        }
        if (pkg.section.branch) {
          formData.append(`package${index + 1}.branch`, pkg.section.branch);
        }
        if (pkg.section.repository) {
          formData.append(`package${index + 1}.repository`, pkg.section.repository);
        }
        if (pkg.section.architecture) {
          formData.append(`package${index + 1}.architecture`, pkg.section.architecture);
        }
      });

      const result = await axios.post(`${process.env.PUBLIC_URL}/api/packages/commit`, formData);

      if (result.data["result"] == "ok") {
        toast.done("Pushed!");
        reload();
      }
    }, [commits, reload])
}

export default (props: any) => {
  const [sections, updateSections] = useSections();

  const [path, setPath] = useState<string[]>(JSON.parse(localStorage.getItem("path") ?? "undefined"));

  useEffect(() => localStorage.setItem("path", JSON.stringify(path)), [path]);

  const [files, updateFiles] = useFilesFromSections(sections, path);

  useEffect(() => updateFiles(sections, path), [sections, path]);

  const [modalCommit, setModalCommit] = useState<ICommit>();
  const [commitModalOpen, setCommitModalOpen] = useState<boolean>(false);

  const [commits, setCommits] = useState<ICommit[]>([]);

  const [isCommitInModalNew, setIsCommitInModalNew] = useState<boolean>(false);

  const [snapshotModalOpen, setSnapshotModalOpen] = useState(false);
  const [snapshotModalBranches, setSnapshotModalBranches] = useState<{
    sourceBranch: string | undefined;
    targetBranch: string | undefined;
  }>({
    sourceBranch: undefined,
    targetBranch: undefined,
  });

  const toggleCommitModalOpen = () => {
    setCommitModalOpen(!commitModalOpen);
  };

  const openModalWithCommitHandler = (isNew: boolean) => {
    return (commit: ICommit) => {
      setIsCommitInModalNew(isNew);
      setModalCommit(commit);
      setCommitModalOpen(true);
    };
  };

  const deleteCommitById = (id: string) => {
    toast.success("Deleted!");
    setCommits(commits.filter((value) => value.id != id));
    setCommitModalOpen(false);
  };

  const openSnapshotModalWithBranchHandler = () => {
    return (sourceBranch?: string, targetBranch?: string) => {
      if (sourceBranch)
        setSnapshotModalBranches({ ...snapshotModalBranches, sourceBranch });
      if (targetBranch)
        setSnapshotModalBranches({ ...snapshotModalBranches, targetBranch });

      setSnapshotModalOpen(true);
    };
  };

  return (
    <div className="flex w-full items-center justify-center font-sans">
      <CommitModal
        isNew={isCommitInModalNew}
        sections={sections}
        onClickBackdrop={toggleCommitModalOpen}
        open={commitModalOpen}
        commit={modalCommit}
        onCommitSubmit={(commit) => {
          setCommitModalOpen(false);
          setCommits([...commits, commit]);
        }}
        onCommitDelete={deleteCommitById}
      />
      <SnapshotModal
        sourceBranch={snapshotModalBranches.sourceBranch}
        targetBranch={snapshotModalBranches.targetBranch}
        sections={sections}
        open={snapshotModalOpen}
        onClickBackdrop={() => setSnapshotModalOpen(false)}
      />

      <Drawer
        open={commits.length > 0}
        mobile={commits.length > 0}
        contentClassName="fm-content"
        side={
          <div>
            <label htmlFor="my-drawer-2" className="drawer-overlay"></label>
            <Menu className="bg-indigo-900 h-screen p-4 w-100 space-y-4">
              <li className="text-3xl font-bold text-white">Pending commits</li>
              {commits?.map((value) => {
                return (
                  <Menu.Item>
                    <CommitCard
                      onActivate={openModalWithCommitHandler(true)}
                      commit={value}
                      onDeleteRequested={deleteCommitById}
                    />
                  </Menu.Item>
                );
              })}
              <div className="grow"></div>
              <Menu.Item>
                <Button type="button" color="accent" onClick={usePushCommitsHandler(commits, () => {
                  setCommits([]);
                  updateSections();
                })}>
                  Push commits
                </Button>
              </Menu.Item>
            </Menu>
          </div>
        }
        end={true}
      >
        <Dropzone
          noClick={true}
          onDrop={usePackageDropHandler(
            path,
            openModalWithCommitHandler(false)
          )}
        >
          {({ getRootProps, getInputProps }) => (
            <section className="h-full">
              <div className="h-full" {...getRootProps()}>
                <input {...getInputProps()} />
                <FullFileBrowser
                  fileActions={[SnapshotAction, SnapToAction]}
                  files={files}
                  onFileAction={useFileActionHandler(
                    setPath,
                    openSnapshotModalWithBranchHandler()
                  )}
                  folderChain={useFolderChainForPath(path)}
                />
              </div>
            </section>
          )}
        </Dropzone>
      </Drawer>
    </div>
  );
};
