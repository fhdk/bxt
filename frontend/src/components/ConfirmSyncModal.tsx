import { forwardRef } from "react";
import { Button, Modal } from "react-daisyui";
import { ModalProps } from "react-daisyui/dist/Modal/Modal";

export interface IConfirmSyncModal {
    onConfirm?: () => void;
    onCancel?: () => void;
}

export default forwardRef<HTMLDialogElement, IConfirmSyncModal>(
    (props: IConfirmSyncModal & React.ComponentProps<typeof Modal>, ref) => {
        return (
            <Modal {...props} backdrop={true} ref={ref}>
                <Modal.Header>Warning</Modal.Header>
                <Modal.Body>
                    Performing a synchronization from the remote repository is a
                    lengthy operation and may disrupt dependencies of overlay
                    packages. Please proceed with caution to avoid unintended
                    side effects.
                </Modal.Body>
                <Modal.Actions>
                    <Button
                        onClick={() => {
                            if (props.onConfirm) props.onConfirm();
                        }}
                        color="ghost"
                    >
                        Do the sync
                    </Button>

                    <Button
                        color="primary"
                        onClick={() => {
                            if (props.onCancel) props.onCancel();
                        }}
                    >
                        Cancel
                    </Button>
                </Modal.Actions>
            </Modal>
        );
    }
);
