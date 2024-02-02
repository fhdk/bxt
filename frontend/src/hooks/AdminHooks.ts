import axios from "axios";
import { useCallback, useEffect, useState } from "react";

export const useUsers = (): [IUser[] | undefined, () => void] => {
    const [users, setUsers] = useState<IUser[]>();

    const updateUsers = useCallback(async () => {
        const users = await axios.get<IUser[]>("/api/users");
        if (users) setUsers(users.data);
    }, [setUsers]);

    useEffect(() => {
        updateUsers();
    }, []);

    return [users, updateUsers];
};
