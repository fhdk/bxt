interface IPackage {
  section: ISection;
  name: string;

  hasSignature?: boolean;
  version?: string;
}

interface IPackageUpload extends IPackage {
  file: File;
  signatureFile?: File;
}