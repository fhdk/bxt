import { useCallback, useEffect, useState } from "react";
import packages from "./packages.json";
import axios from "axios";

export interface IUpdateSections {
    (): void;
}

export const useSections = (): [ISection[], IUpdateSections] => {
    const [sections, setSections] = useState<ISection[]>([]);

    const updateSections: IUpdateSections = useCallback(() => {
        axios
            .get(`${process.env.PUBLIC_URL}/api/sections/get`)
            .then((response) => {
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
        axios
            .get(`${process.env.PUBLIC_URL}/api/logs/packages`)
            .then((response) => {
                const entries = response.data.map((value: any) => {
                    value.time = new Date(value.time);
                    return value;
                });
                setEntries(entries);
            });
    }, []);

    return entries;
};

export interface IGetCompareResults {
    (sections: ISection[]): void;
}

export const useCompareResults = (): [
    ICompareResult | undefined,
    IGetCompareResults
] => {
    const [results, setResults] = useState<ICompareResult>();

    const updateResults: IGetCompareResults = useCallback(
        async (sections: ISection[]) => {
            const result = await axios.post(
                `${process.env.PUBLIC_URL}/api/compare`,
                sections
            );

            const compareEntries: ICompareEntry[] = [];

            Object.keys(result.data["compare_table"]).forEach((value) => {
                const versions = { ...result.data["compare_table"] }[value];

                compareEntries.push({ name: value, ...versions });
            });

            setResults({
                sections: result.data.sections,
                compareTable: compareEntries
            });
        },
        [setResults]
    );

    return [results, updateResults];
};
