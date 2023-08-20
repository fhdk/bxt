import { Table } from "react-daisyui";
import logs from "./logs.json";
import { usePackageLogs } from "../hooks/BxtHooks";

export default (props: any) => {

  const entries = usePackageLogs();

  return (
    <div className='w-full overflow-x-auto'>
      <Table zebra={true} className='w-full' {...props}>
        <Table.Head>
          <span />
          <span>Id</span>
          <span>Package</span>
          <span>Time</span>
        </Table.Head>

        <Table.Body>
          {entries.map((value, index) => {
            return (
              <Table.Row>
                <span>{index}</span>
                <span>{value.id}</span>
                <span>{value.package}</span>
                <span>{value.time.toLocaleString()}</span>
              </Table.Row>
            );
          })}
        </Table.Body>
      </Table>
    </div>
  );
};
