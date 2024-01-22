interface IPackagePoolEntry {
    version: string;
    filepath: string;
    signaturePath?: string;
}

interface IPackage {
    section: ISection;
    name: string;
    isAnyArchitecture?: boolean;
    preferredCandidate?: IPackagePoolEntry;
    poolEntries?: {
        [key: string]: IPackagePoolEntry;
    };
}

interface IPackageUpload {
    section: ISection;
    name: string;
    version?: string;
    file: File;
    signatureFile?: File;
}
