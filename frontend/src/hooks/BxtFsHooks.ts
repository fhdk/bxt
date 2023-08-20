import { FileArray, FileData } from "chonky";
import { useCallback, useEffect, useMemo, useState } from "react";
import {
  architecturesForBranchAndRepo,
  branches,
  reposForBranch,
} from "../utils/SectionUtils";
import packages from "./packages.json";
import axios from "axios";


interface IUpdateFiles {
  (sections: ISection[],
    path: string[]): void;
}

export const useFilesFromSections = (
  sections: ISection[],
  path: string[]
): [FileArray, IUpdateFiles] => {

  const [files, setFiles] = useState<FileArray>([]);

  const updateFiles = useCallback((sections: ISection[],
    path: string[]) => {
    switch (path.length) {
      case 1:
        setFiles(branches(sections).map((value): FileData => {
          return {
            id: `root/${value}`,
            name: value,
            isDir: true,
            thumbnailUrl: `${process.env.PUBLIC_URL}/code-branch-solid.svg`,
          };
        }));
        break;
      case 2:
        setFiles(reposForBranch(sections, path[1]).map((value): FileData => {
          return {
            id: `root/${path[1]}/${value}`,
            name: value,
            isDir: true,
            thumbnailUrl: `${process.env.PUBLIC_URL}/cubes-solid.svg`,
          };
        }));
        break;
      case 3:
        setFiles(architecturesForBranchAndRepo(sections, path[1], path[2]).map(
          (value): FileData => {
            return {
              id: `root/${path[1]}/${path[2]}/${value}`,
              name: value,
              isDir: true,
              thumbnailUrl: `${process.env.PUBLIC_URL}/microchip-solid.svg`,
            };
          }
        ));
        break;
      case 4:
        axios.get(`${process.env.PUBLIC_URL}/api/packages/get`, { params: { branch: path[1], repository: path[2], architecture: path[3] } }).then((value) => {
          if (value.data == null) {
            setFiles([]);
            return;
          }
          setFiles(value.data.map((value: any) => ({
            id: `root/${path[1]}/${path[2]}/${path[3]}/${value?.filename}`,
            name: value!.filename,
            isDir: false,
            thumbnailUrl: `${process.env.PUBLIC_URL}/cube-solid.svg`,
          })));
        });
        break;
    }
  }, [sections, path, setFiles]);

  useEffect(() => { updateFiles(sections, path) }, [sections, path]);

  return [files, updateFiles];

};
