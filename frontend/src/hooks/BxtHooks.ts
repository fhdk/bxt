import { useCallback, useEffect, useState } from "react";
import packages from "./packages.json";
import axios from "axios";

export interface IUpdateSections {
  (): void;
}

export const useSections = (): [ISection[], IUpdateSections] => {
  const [sections, setSections] = useState<ISection[]>([]);

  const updateSections: IUpdateSections = useCallback(() => {
    axios.get(`${process.env.PUBLIC_URL}/api/sections/get`).then((response) => {
      setSections(response.data);
    });

  }, [setSections]);

  useEffect(() => {
    updateSections();
  }, []);

  return [sections, updateSections];
};


export const usePackageLogs = (): ILogEntry[] => {
  const [entries, setEntries] = useState<ILogEntry[]>([]);

  useEffect(() => {
    axios.get(`${process.env.PUBLIC_URL}/api/logs/packages`).then((response) => {

      const entries = response.data.map((value: any) => {
        value.time = new Date(value.time);
        return value;
      })
      setEntries(entries);
    });
  }, [])

  return entries;
}