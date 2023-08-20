import axios from "axios";
import { useState } from "react";
import { Hero, Button, Card, Form, Input } from "react-daisyui";

export default (props: any) => {
  const [token, setToken] = useState("");
  const [password, setPassword] = useState("");
  const [name, setName] = useState("");

  return (
    <div className="flex w-full component-preview p-4 items-center justify-center gap-2 font-sans">
      <Hero className="grid h-screen place-items-center" {...props}>
        <Hero.Content className="flex-col lg:flex-row-reverse">
          <Card className="flex-shrink-0 w-full max-w-sm shadow-2xl bg-base-100">
            <Card.Body>
              <div className="justify-start relative  h-20  overflow-hidden pb-2/3">
                <img
                  src={`${process.env.PUBLIC_URL}/logo-full.png`}
                  className="absolute h-full w-full object-contain"
                />
              </div>
              <Form>
                <Form.Label title="Login" />
                <Input
                  value={name}
                  onChange={(e) => setName(e.target.value)}
                  type="text"
                  placeholder="login"
                  className="input-bordered"
                />
              </Form>
              <Form>
                <Form.Label title="Password" />
                <Input
                  value={password}
                  onChange={(e) => setPassword(e.target.value)}
                  type="password"
                  placeholder="password"
                  className="input-bordered"
                />
              </Form>
              <Form className="mt-6">
                <Button
                  onClick={() => {
                    axios
                      .post("http://localhost:8080/auth", {
                        name: name,
                        password: password,
                      })
                      .then((res) => {
                        console.log(res.data);
                      });
                  }}
                  type="button"
                  color="primary"
                >
                  Login
                </Button>
              </Form>
            </Card.Body>
          </Card>
        </Hero.Content>
      </Hero>
    </div>
  );
};
