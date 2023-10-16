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
import { Button, Select, Table } from "react-daisyui";
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
    faTrash
} from "@fortawesome/free-solid-svg-icons";

export default (props: any) => {
    const [sections, updateSections] = useSections();
    const [selectedSections, setSelectedSections] = useState<ISection[]>([]);
    const [compareResults, getCompareResults] = useCompareResults();
    const [plainTextSelectorMode, setPlainTextSelectorMode] = useState<
        Map<ISection, boolean>
    >(new Map());

    const updateSectionAt = (index: number, update: any) => {
        setSelectedSections((prevSections) => [
            ...prevSections.slice(0, index),
            { ...sections[0], ...prevSections[index], ...update },
            ...prevSections.slice(index + 1)
        ]);
    };

    useEffect(() => {
        getCompareResults([]);
    }, [sections]);

    useEffect(() => {
        setSelectedSections(compareResults?.sections || []);
    }, [compareResults]);

    const columnHelper = createColumnHelper<ICompareEntry>();

    const columns = [
        columnHelper.accessor("name", {
            id: "0",
            header: "Name"
        }),
        ...selectedSections.map((section, index) =>
            columnHelper.accessor(
                (compare: ICompareEntry) => {
                    if (
                        section.branch &&
                        section.repository &&
                        section.architecture
                    )
                        return compare[
                            `${section.branch}/${section.repository}/${section.architecture}`
                        ];
                },
                {
                    id: `${index + 1}`,
                    header: `${section.branch}/${section.repository}/${section.architecture}`
                }
            )
        )
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
        <div className="w-full h-full overflow-x-auto">
            <div className="p-3 bg-base-200">
                <span className="p-5 space-x-2 inline-flex w-full justify-between">
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
                                className="bg-transparent 
                                    text-blue-dark 
                                    font-semibold 
                                    hover:text-white 
                                    py-2 
                                    border
                                    border-base-content/20
                                    hover:border-transparent 
                                    rounded-none 
                                    rounded-l-lg ml-2"
                                size="sm"
                            >
                                <FontAwesomeIcon icon={faParagraph} />
                            </Button>
                            <Button
                                className="rounded-none rounded-r-lg"
                                size="sm"
                                color="error"
                            >
                                <FontAwesomeIcon icon={faTrash} />
                            </Button>
                        </div>
                    ))}
                </span>
                <div className="w-full flex justify-end">
                    <Button color="primary">
                        <FontAwesomeIcon
                            className="mr-2"
                            icon={faCodeCompare}
                        />
                        Compare
                    </Button>
                </div>
            </div>
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
                                              header.column.columnDef.header,
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
        </div>
    );
};
