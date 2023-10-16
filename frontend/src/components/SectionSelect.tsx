import { Input, Select } from "react-daisyui";
import {
    branches,
    reposForBranch,
    architecturesForBranchAndRepo
} from "../utils/SectionUtils";
import { useSections } from "../hooks/BxtHooks";
import React, { HTMLAttributes, useCallback, useEffect, useState } from "react";

type ISectionSelectorProps = HTMLAttributes<HTMLDivElement> & {
    sections: ISection[];
    selectedSection?: ISection;
    onSelected?: (section: ISection | undefined) => void;
    plainTextMode?: boolean;
};

export default (props: ISectionSelectorProps) => {
    const [plainTextInput, setPlainTextInput] = useState<string>();

    useEffect(() => {
        setPlainTextInput(
            `${props.selectedSection?.branch}/${props.selectedSection?.repository}/${props.selectedSection?.architecture}`
        );
    }, [props.selectedSection]);

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

            if (props.onSelected) props.onSelected(section);
        },
        [props.onSelected]
    );

    const updateSection = useCallback(
        (update: any) => {
            if (props.onSelected)
                props.onSelected({ ...props.selectedSection, ...update });
        },
        [props.onSelected, props.sections]
    );

    useEffect(() => {
        if (props.onSelected)
            props.onSelected({
                ...props.sections[0],
                ...props.selectedSection
            });
    }, [props.sections]);

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
                        value={props.selectedSection?.branch}
                        size="sm"
                    >
                        {branches(props.sections).map((value) => (
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
                        value={props.selectedSection?.repository}
                        size="sm"
                    >
                        {reposForBranch(
                            props.sections,
                            props.selectedSection?.branch
                        ).map((value) => (
                            <option
                                onClick={() =>
                                    updateSection({ repository: value })
                                }
                            >
                                {value}
                            </option>
                        ))}
                    </Select>
                    <span className="px-2 text-xs/6 text-base-content/50">
                        Architecture
                    </span>
                    <Select
                        className="rounded-none rounded-r-lg"
                        value={props.selectedSection?.architecture}
                        size="sm"
                    >
                        {architecturesForBranchAndRepo(
                            props.sections,
                            props.selectedSection?.branch,
                            props.selectedSection?.repository
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
