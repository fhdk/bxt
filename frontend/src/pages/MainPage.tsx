import { Drawer, Menu } from "react-daisyui";
import FileViewPage from "./FileViewPage";
import {
  RouterProvider,
  createBrowserRouter,
  Link,
  Outlet
} from "react-router-dom";
import LogPage from "./LogPage";
import { ToastContainer, toast } from "react-toastify";
import { useLocalStorage } from "@uidotdev/usehooks";
import LoginPage from "./LoginPage";
import { useEffect, useState } from "react";

declare module "@uidotdev/usehooks" {
  export function useLocalStorage<T>(
    key: string,
    initialValue: T
  ): [T, (v: T) => void];
}

export default (props: any) => {
  const [userName, setUserName] = useLocalStorage("username", null);

  const DrawerLayout = () => {
    return <Drawer
      mobile={true}
      side={
        <Menu className="text-neutral h-screen p-4 w-60 bg-primary text-base-content">
          <Menu.Item>
            <Link
              to="/"
              className="justify-start relative  h-14  overflow-hidden pb-2/3"
            >
              <img
                src={`${process.env.PUBLIC_URL}/logo-full.png`}
                className="absolute h-full w-40 object-contain"
              />
            </Link>
          </Menu.Item>
          <div className="h-6"></div>
          <Menu.Item>
            <Link to="/compare">Compare</Link>
          </Menu.Item>
          <Menu.Item>
            <Link to="/logs">Logs</Link>
          </Menu.Item>
          <div className="grow"></div>
          <Menu.Item>
            <Link to="/admin">Admin</Link>
          </Menu.Item>
          <Menu.Item>
            <a>Logout</a>
          </Menu.Item>
        </Menu>}>
      <Outlet />
    </Drawer>
  }

  const router = createBrowserRouter([
    {
      element: <DrawerLayout />,
      children: [
        {
          path: "",
          element: <FileViewPage style={{ zIndex: 10 }} />,
        },
        {
          path: "logs",
          element: <LogPage />,
        },
      ]
    },


  ]);


  if (!userName) {
    return (
      <div className="flex w-full items-center justify-center font-sans">
        <ToastContainer />
        <LoginPage />
      </div>)
  }

  return (
    <div className="flex w-full items-center justify-center font-sans">
      <ToastContainer />
      <RouterProvider router={router} />
    </div>
  );
};

