import FileViewPage from "./FileViewPage";
import { RouterProvider, createBrowserRouter } from "react-router-dom";
import LogPage from "./LogPage";
import { ToastContainer } from "react-toastify";
import { useLocalStorage } from "@uidotdev/usehooks";
import LoginPage from "./LoginPage";
import ComparePage from "./ComparePage";
import DrawerLayout from "../components/DrawerLayout";

declare module "@uidotdev/usehooks" {
    export function useLocalStorage<T>(
        key: string,
        initialValue: T
    ): [T, (v: T) => void];
}
export default (props: any) => {
    const [userName, setUserName] = useLocalStorage("username", null);

    const router = createBrowserRouter([
        {
            element: <DrawerLayout />,
            children: [
                {
                    path: "",
                    element: <FileViewPage style={{ zIndex: 10 }} />
                },
                {
                    path: "logs",
                    element: <LogPage />
                },
                {
                    path: "compare",
                    element: <ComparePage />
                }
            ]
        }
    ]);

    return (
        <div className="flex w-full items-center justify-center font-sans">
            <ToastContainer />

            {userName != null ? (
                <RouterProvider router={router} />
            ) : (
                <LoginPage />
            )}
        </div>
    );
};
