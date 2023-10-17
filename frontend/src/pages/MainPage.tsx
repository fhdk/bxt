import FileViewPage from "./FileViewPage";
import { RouterProvider, createBrowserRouter } from "react-router-dom";
import LogPage from "./LogPage";
import { ToastContainer, toast } from "react-toastify";
import { useLocalStorage } from "@uidotdev/usehooks";
import LoginPage from "./LoginPage";
import ComparePage from "./ComparePage";
import DrawerLayout from "../components/DrawerLayout";
import axios from "axios";

declare module "@uidotdev/usehooks" {
    export function useLocalStorage<T>(
        key: string,
        initialValue: T
    ): [T, (v: T) => void];
}
export default (props: any) => {
    const [userName, setUserName] = useLocalStorage("username", null);

    axios.interceptors.response.use(
        (response) => response,
        (error) => {
            toast.error(`Response error: ${error.response?.data?.error}`, {
                autoClose: false
            });
        }
    );

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
