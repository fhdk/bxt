interface ICompareEntry {
    name: string;
    [section: string]: {
        [location: string]: string;
    };
}

interface ICompareResult {
    sections: ISection[];
    compareTable: ICompareEntry[];
}
