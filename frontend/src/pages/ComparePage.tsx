import React, { useState, useEffect, useMemo } from "react";
import {
    createColumnHelper,
    flexRender,
    getCoreRowModel,
    getPaginationRowModel,
    getSortedRowModel,
    useReactTable
} from "@tanstack/react-table";
import { useCompareResults, useSections } from "../hooks/BxtHooks";
import { Button, Loading, RadialProgress, Select, Table } from "react-daisyui";
import {
    architecturesForBranchAndRepo,
    branches,
    reposForBranch
} from "../utils/SectionUtils";
import SectionSelect from "../components/SectionSelect";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import {
    faCodeCompare,
    faFileText,
    faKeyboard,
    faParagraph,
    faPlus,
    faTrash
} from "@fortawesome/free-solid-svg-icons";

export default (props: any) => {
    const [sections, updateSections] = useSections();
    const [selectedSections, setSelectedSections] = useState<ISection[]>([]);
    const [compareResults, getCompareResults] = useCompareResults();
    const [plainTextSelectorMode, setPlainTextSelectorMode] = useState<
        Map<ISection, boolean>
    >(new Map());

    const [isLoading, setIsLoading] = useState<boolean>(false);

    const updateSectionAt = (index: number, update: any) => {
        setSelectedSections((prevSections) => [
            ...prevSections.slice(0, index),
            { ...sections[0], ...prevSections[index], ...update },
            ...prevSections.slice(index + 1)
        ]);
    };

    useEffect(() => {
        setIsLoading(false);
    }, [compareResults]);

    const columnHelper = createColumnHelper<ICompareEntry>();

    const columns = [
        columnHelper.accessor("name", {
            id: "0",
            header: "Name"
        }),
        ...(compareResults?.sections.map((section, index) =>
            columnHelper.accessor(
                (compare: ICompareEntry) => {
                    if (
                        section.branch &&
                        section.repository &&
                        section.architecture
                    )
                        return Object.values(
                            compare[
                                `${section.branch}/${section.repository}/${section.architecture}`
                            ]
                        );
                },
                {
                    id: `${index + 1}`,
                    header: (props) => (
                        <SectionLabel {...props} section={section} />
                    )
                }
            )
        ) || [])
    ];

    const data = useMemo(
        () => compareResults?.compareTable || [],
        [compareResults]
    );

    const table = useReactTable<ICompareEntry>({
        columns,
        data,
        manualPagination: true,
        getCoreRowModel: getCoreRowModel(),
        getPaginationRowModel: getPaginationRowModel(),
        getSortedRowModel: getSortedRowModel()
    });

    return (
        <div className="flex flex-col w-full h-full overflow-x-auto">
            <div className="grow-0 p-3 bg-base-200">
                <span className="p-5 space-x-2 inline-flex w-full justify-start">
                    {selectedSections.map((section, index) => (
                        <div className="flex items-end h-fit" key={index}>
                            <SectionSelect
                                sections={sections}
                                selectedSection={section}
                                plainTextMode={plainTextSelectorMode?.get(
                                    section
                                )}
                                onSelected={(section) =>
                                    updateSectionAt(index, section)
                                }
                            />
                            <Button
                                onClick={(e) => {
                                    setPlainTextSelectorMode(
                                        new Map(
                                            plainTextSelectorMode?.set(
                                                section,
                                                !plainTextSelectorMode?.get(
                                                    section
                                                )
                                            )
                                        )
                                    );
                                }}
                                className="py-2 
                                    rounded-none 
                                    rounded-l-lg ml-2"
                                size="sm"
                                variant="outline"
                            >
                                <FontAwesomeIcon icon={faParagraph} />
                            </Button>
                            <Button
                                className="rounded-none rounded-r-lg"
                                size="sm"
                                color="error"
                                onClick={() =>
                                    setSelectedSections([
                                        ...selectedSections.slice(0, index),
                                        ...selectedSections.slice(index + 1)
                                    ])
                                }
                            >
                                <FontAwesomeIcon icon={faTrash} />
                            </Button>
                        </div>
                    ))}
                </span>
                <div className="w-full flex justify-end space-x-2">
                    <Button
                        color="neutral"
                        onClick={() =>
                            setSelectedSections([...selectedSections, {}])
                        }
                    >
                        <FontAwesomeIcon className="mr-2" icon={faPlus} />
                        Add section
                    </Button>
                    <Button
                        color="primary"
                        onClick={() => {
                            setIsLoading(true);
                            getCompareResults(selectedSections);
                        }}
                    >
                        <FontAwesomeIcon
                            className="mr-2"
                            icon={faCodeCompare}
                        />
                        Compare
                    </Button>
                </div>
            </div>
            {isLoading ? (
                <div className="grow flex w-full justify-center content-center">
                    <Loading variant="bars" className="w-20" />
                </div>
            ) : (
                <Table zebra={true} size="xs" className="w-full">
                    <Table.Head>
                        {table
                            .getHeaderGroups()
                            .flatMap((headerGroup) =>
                                headerGroup.headers.map((header) => (
                                    <span key={header.id}>
                                        {header.isPlaceholder
                                            ? null
                                            : flexRender(
                                                  header.column.columnDef
                                                      .header,
                                                  header.getContext()
                                              )}
                                    </span>
                                ))
                            )}
                    </Table.Head>
                    <Table.Body>
                        {table.getRowModel().rows.map((row) => (
                            <Table.Row key={row.id}>
                                {row.getVisibleCells().map((cell) => (
                                    <span key={cell.id}>
                                        {flexRender(
                                            cell.column.columnDef.cell,
                                            cell.getContext()
                                        )}
                                    </span>
                                ))}
                            </Table.Row>
                        ))}
                    </Table.Body>
                </Table>
            )}
        </div>
    );
};
