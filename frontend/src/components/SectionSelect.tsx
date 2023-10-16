import { Input, Select } from "react-daisyui";
import {
    branches,
    reposForBranch,
    architecturesForBranchAndRepo
} from "../utils/SectionUtils";
import { useSections } from "../hooks/BxtHooks";
import React, { useCallback, useEffect, useState } from "react";

interface ISectionSelectorProps extends React.HTMLAttributes<HTMLDivElement> {
    onSelected?: (section: ISection | undefined) => void;
    plainTextMode?: boolean;
}

export default (props: ISectionSelectorProps) => {
    const [sections, updateSections] = useSections();
    const [selectedSection, setSelectedSection] = useState<ISection>();

    const [plainTextInput, setPlainTextInput] = useState<string>();

    useEffect(() => {
        setPlainTextInput(
            `${selectedSection?.branch}/${selectedSection?.repository}/${selectedSection?.architecture}`
        );
    }, [selectedSection]);

    const setSectionFromPlainText = useCallback(
        (plainTextSection: string | undefined) => {
            if (!plainTextSection) {
                return;
            }
            const section: ISection = {};
            const parts = plainTextSection.split("/");

            section.branch = parts[0];
            section.repository = parts[1];
            section.architecture = parts[2];

            setSelectedSection(section);
        },
        [setSelectedSection]
    );

    const updateSection = useCallback(
        (update: any) => {
            setSelectedSection({ ...selectedSection, ...update });
        },
        [setSelectedSection, sections]
    );

    useEffect(() => {
        if (props.onSelected) props.onSelected(selectedSection);
    }, [selectedSection]);

    useEffect(() => {
        setSelectedSection({ ...sections[0], ...selectedSection });
    }, [sections]);

    return (
        <div className="w-72" {...props}>
            {props.plainTextMode ? (
                <div className="flex h-[51px] items-end">
                    <Input
                        className="inline-block w-full"
                        size="sm"
                        value={plainTextInput}
                        onChange={(e) => setPlainTextInput(e.target.value)}
                        onKeyDown={(e) => {
                            if (e.key == "Enter") {
                                setSectionFromPlainText(plainTextInput);
                            }
                        }}
                    />
                </div>
            ) : (
                <span className="h-[51px] inline-block grid grid-flow-col grid-cols-3 grid-rows-[19px_24px] justify-stretch w-full">
                    <span className="px-2 text-xs/6 text-base-content/50">
                        Branch
                    </span>
                    <Select
                        className="rounded-none rounded-l-lg"
                        value={selectedSection?.branch}
                        size="sm"
                    >
                        {branches(sections).map((value) => (
                            <option
                                onClick={() => updateSection({ branch: value })}
                            >
                                {value}
                            </option>
                        ))}
                    </Select>
                    <span className="px-2 text-xs/6 text-base-content/50">
                        Repository
                    </span>
                    <Select
                        className="rounded-none"
                        value={selectedSection?.repository}
                        size="sm"
                    >
                        {reposForBranch(sections, selectedSection?.branch).map(
                            (value) => (
                                <option
                                    onClick={() =>
                                        updateSection({ repository: value })
                                    }
                                >
                                    {value}
                                </option>
                            )
                        )}
                    </Select>
                    <span className="px-2 text-xs/6 text-base-content/50">
                        Architecture
                    </span>
                    <Select
                        className="rounded-none rounded-r-lg"
                        value={selectedSection?.architecture}
                        size="sm"
                    >
                        {architecturesForBranchAndRepo(
                            sections,
                            selectedSection?.branch,
                            selectedSection?.repository
                        ).map((value) => (
                            <option
                                onClick={() =>
                                    updateSection({ architecture: value })
                                }
                            >
                                {value}
                            </option>
                        ))}
                    </Select>
                </span>
            )}
        </div>
    );
};
