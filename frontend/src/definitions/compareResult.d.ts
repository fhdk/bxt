interface ICompareEntry {
  name: string,
  [section: string]: string
}

interface ICompareResult {
  sections: ISection[];
  compareTable: ICompareEntry[];
}

