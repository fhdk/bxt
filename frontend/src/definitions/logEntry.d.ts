interface ILogEntry {
    id: string;

    action: string;
    package: IPackage;
    time: Date;
}
