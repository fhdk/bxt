import { Drawer, Menu } from "react-daisyui";
import FileViewPage from "./FileViewPage";
import {
  RouterProvider,
  createBrowserRouter,
} from "react-router-dom";
import LogPage from "./LogPage";
import { ToastContainer } from "react-toastify";

export default (props: any) => {
  const side = (
    <div>
      <label htmlFor="my-drawer-2" className="drawer-overlay"></label>
      <Menu className="text-neutral h-screen p-4 w-60 bg-primary text-base-content">
        <Menu.Item>
          <a
            href="/"
            className="justify-start relative  h-20  overflow-hidden pb-2/3"
          >
            <img
              src={`${process.env.PUBLIC_URL}/logo-full.png`}
              className="absolute h-full w-40 object-contain"
            />
          </a>
        </Menu.Item>

        <Menu.Item>
          <a>Compare</a>
        </Menu.Item>
        <Menu.Item>
          <a href="/logs">Logs</a>
        </Menu.Item>
        <div className="grow"></div>
        <Menu.Item>
          <a>Admin</a>
        </Menu.Item>
      </Menu>
    </div>
  );

  const router = createBrowserRouter([
    {
      path: "/",
      element: <FileViewPage />,
    },
    {
      path: "/logs",
      element: <LogPage />,
    },
  ]);

  return (
    <div className="flex w-full items-center justify-center font-sans">
      <ToastContainer />
      <Drawer side={side} {...props} mobile={true}>
        <RouterProvider router={router} />
      </Drawer>
    </div>
  );
};
