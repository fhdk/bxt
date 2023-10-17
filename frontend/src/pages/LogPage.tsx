import { Loading, Table } from "react-daisyui";
import logs from "./logs.json";
import { usePackageLogs } from "../hooks/BxtHooks";
import { useEffect, useState } from "react";

const dateCompare = (a: ILogEntry, b: ILogEntry): number => {
    if (a.time < b.time) {
        return -1;
    }
    if (a.time > b.time) {
        return 1;
    }
    return 0;
};

export default (props: any) => {
    const [entries, updateEntries] = usePackageLogs();
    const [isLoading, setIsLoading] = useState<boolean>(false);

    useEffect(() => {
        updateEntries();
        setIsLoading(true);
    }, []);

    useEffect(() => {
        setIsLoading(false);
    }, [entries]);

    return (
        <div className="flex flex-col w-full h-full overflow-x-auto">
            {isLoading ? (
                <div className="grow flex w-full justify-center content-center">
                    <Loading variant="bars" className="w-20" />
                </div>
            ) : (
                <Table zebra={true} className="w-full" {...props}>
                    <Table.Head>
                        <span />
                        <span>Id</span>
                        <span>Package</span>
                        <span>Time</span>
                    </Table.Head>

                    <Table.Body>
                        {entries
                            .sort((a, b) => -dateCompare(a, b))
                            .map((value, index) => {
                                return (
                                    <Table.Row>
                                        <span>{index}</span>
                                        <span>{value.id}</span>
                                        <span>{value.package}</span>
                                        <span>
                                            {value.time.toLocaleString()}
                                        </span>
                                    </Table.Row>
                                );
                            })}
                    </Table.Body>
                </Table>
            )}
        </div>
    );
};
