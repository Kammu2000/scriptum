// imports
import Environment from "../runtime/environment"
import { registerGlobals } from "./globals"

export const loadStdlib = (env: Environment): void  => {
  registerGlobals(env);
  return;
}

