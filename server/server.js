import Koa from "koa";
import Router from "koa-router";
import json from "koa-json";
import logger from "koa-logger";
import body from "koa-body";
import child from "child_process";
import {readFile}  from "fs/promises"
const promisify = (fn, receiver) => {
  return (...args) => {
    return new Promise((resolve, reject) => {
      fn.apply(receiver, [
        ...args,
        (err, res) => {
          return err ? reject(err) : resolve(res);
        },
      ]);
    });
  };
};
const execPromise = promisify(child.execFile, child);
console.log(child);
const app = new Koa();
const r = new Router();
app.use(json());
app.use(logger());
app.use(
  body({
    // json:true,
    multipart: true,
    parsedMethods: ["POST", "PUT", "PATCH", "GET", "HEAD", "DELETE"],
  })
);

app.on("error", function (err) {
  console.log("server error", err);
});

r.get("/api/solve", async (ctx) => {
  const exe = +ctx.query.isOptimize ? "main2.exe" : "main1.exe";
  let res = await execPromise(exe, [ctx.query.file], {
    cwd: "./test",
    timeout: 60 * 1000,
  });
  let f = "./test/" + /(?<=(:\s)).*\.res/.exec(res)[0];
  let file = await readFile(f,'utf-8');
  ctx.body = {
    msg: res,
    file,
  };
});
r.get("/api/sudoku", async (ctx) => {
  const exe = +ctx.query.isOptimize ? "main2.exe" : "main1.exe";
  let res = await execPromise(exe, [], {
    cwd: "./test",
    timeout: 60 * 1000,
  });
  ctx.body = {
    msg: res.split(/\s\r\n\r\n/),
  };
});

app.use(r.routes());
app.use(r.allowedMethods());
app.listen(8080, () => {
  console.log("Koa is listening in 8080");
}).timeout = 60 * 1000;

export default app;
