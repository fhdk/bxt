import { Drawer, Menu, Button, Progress } from "react-daisyui";
import { Link, Outlet } from "react-router-dom";
import ConfirmSyncModal from "./ConfirmSyncModal";
import { useCallback, useRef, useState } from "react";
import { useLocalStorage } from "@uidotdev/usehooks";
import { createPortal } from "react-dom";
import { ProgressBar } from "react-toastify/dist/components";

export default () => {
    let modalRef = useRef<HTMLDialogElement>(null);
    const [userName, setUserName] = useLocalStorage("username", null);
    const [syncInProgress, setSyncInProgress] = useState<boolean>(false);

    const handleShow = useCallback(() => {
        modalRef.current?.showModal();
    }, [modalRef]);

    return (
        <Drawer
            className="lg:drawer-open"
            open={true}
            side={
                <Menu className="text-neutral h-screen p-4 w-60 bg-primary text-base-content">
                    <Link
                        to="/"
                        className="flex justify-center relative  h-14  overflow-hidden pb-2/3"
                    >
                        <img
                            id="bxt-logo"
                            src={`${process.env.PUBLIC_URL}/logo-full.png`}
                            className="absolute h-full w-40 object-contain"
                        />
                    </Link>

                    <div className="h-6"></div>

                    <Menu.Item>
                        <Link to="/compare">Compare</Link>
                    </Menu.Item>

                    <Menu.Item>
                        <Link to="/logs">Logs</Link>
                    </Menu.Item>
                    <div className="grow"></div>
                    {syncInProgress ? (
                        <div className="font-bold text-center">
                            Sync is in progress
                            <Progress />
                        </div>
                    ) : (
                        <Button size="sm" onClick={handleShow} color="accent">
                            Sync
                        </Button>
                    )}

                    <div className="h-5 flex flex-col place-content-center">
                        <hr />
                    </div>
                    <Menu.Item onClick={(e) => setUserName(null)}>
                        <a>Logout</a>
                    </Menu.Item>
                </Menu>
            }
        >
            <ConfirmSyncModal
                onCancel={() => modalRef.current?.close()}
                onConfirm={() => {
                    setSyncInProgress(true);
                    modalRef.current?.close();
                }}
                ref={modalRef}
            />

            <Outlet />
        </Drawer>
    );
};
