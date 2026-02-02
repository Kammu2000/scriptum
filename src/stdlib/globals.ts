// imports
import Environment from "../runtime/environment";

const NATIVE_FUNCTION_MAP = {
  print: {
    kind: "native-function",
    arity: null,
    call: function(args: any[], _env: Environment): void {
      console.log(...args);
      return;
    }
  },
  
  type: {
    kind: "native-function",
    arity: 1,
    call: function(args: any[], _env: Environment): string {
      return typeof args[0];
    }
  }
};

export const registerGlobals = (env: Environment): void => {
  for(const [key, value] of Object.entries(NATIVE_FUNCTION_MAP)){
    env.declareVar(key, value);
  }
};

