// imports 
import ProgramParser from "../frontend/parser/programParser";
import { Expression, ExpressionKind, Identifier, Program, Statement, StatementKind } from "../frontend/parser/types";
import Environment from "./environment";
import fs from "node:fs";
import path from "node:path";
import { ReturnSignal } from "./signals";
import { loadStdlib } from "../stdlib";

function evaluate(expr: Expression, env: Environment | null): any {

  switch (expr.kind) {
    case ExpressionKind.NumericLiteral:
      return expr.value;  
    
    case ExpressionKind.Identifier: 
      return env?.lookupVar(expr.symbol);

    case ExpressionKind.AssignmentExpression: {
      const { left, right } = expr;

      const computedValue = evaluate(right, env);
      env?.assignVar(left.symbol, computedValue); 

      return computedValue; 
    }

    case ExpressionKind.CallExpression: {
      const { callee, args } = expr;
      const fnValue = evaluate(callee, env);

      if(!fnValue || typeof fnValue !== "object" || (fnValue.kind !== "native-function" && fnValue.kind !== "function")){
        throw new Error("No such function exists");
      }

      const evaluatedArgs = args.map((arg: Expression): any => evaluate(arg, env));

      if(fnValue.kind === "native-function"){
        if(fnValue.arity && fnValue.arity !== args.length){
          throw new Error("Function arguments should have same length as params");
        }

        return fnValue.call(evaluatedArgs, env);
      }

      const { env: parentEnv, params, body } = fnValue;


      if(params.length !== args.length){
        throw new Error("Function arguments should have same length as params");
      }
      
      const callEnv = new Environment(parentEnv);

      for(let i = 0; i < evaluatedArgs.length; i++){
        callEnv.declareVar(params[i], evaluatedArgs[i]);
      }
     
      try {
        execute(body, callEnv); 
      } catch (e: unknown) {
        if(e instanceof ReturnSignal){ 
          return e.value;
        }
        
        throw e;
      }

      return undefined;
    }

    case ExpressionKind.BinaryExpression: {
      const leftValue = evaluate(expr.left, env);
      const rightValue = evaluate(expr.right, env);

      switch (expr.op) {
        case "+":
          return leftValue + rightValue;
        
        case "-":
          return leftValue - rightValue;

        case "*":
          return leftValue * rightValue;

        case "/":
          return leftValue / rightValue;

        case "%":
          return leftValue % rightValue;
       
        case ">":
          return Number(leftValue > rightValue);

        case ">=":
          return Number(leftValue >= rightValue);

        case "<":
          return Number(leftValue < rightValue);

        case "<=":
          return Number(leftValue <= rightValue);

        case "===":
          return Number(leftValue === rightValue);

        case "!==":
          return Number(leftValue !== rightValue);

        default: {
          throw new Error(`Could not identify the operator ${expr.op}`);
        }
      }
    }

    default: {
      throw new Error("Invalid expression");
    }
  }
  
}

function execute(stmt: Statement, env: Environment | null): void {
  switch (stmt.kind) {
    case StatementKind.ExpressionStatement: {
      const { expression } = stmt;
      const val = evaluate(expression, env); 
      return;
    }

    case StatementKind.VariableDeclaration: {
      const { id, init } = stmt;
      const val = init !== undefined ? evaluate(init, env): init;
      env?.declareVar(id.symbol, val);
      return; 
    }
      
    case StatementKind.IfStatement: {
      const { test, thenBlock, elseBlock } = stmt;
      
      if(evaluate(test, env)){
        execute(thenBlock, env);
      }
      else {
        if(elseBlock)
          execute(elseBlock, env);
      }

      return;
    }

    case StatementKind.WhileStatement: {
      const { test, body } = stmt;
      
      while(evaluate(test, env)){
        execute(body, env)
      }

      return;
    }

    case StatementKind.BlockStatement: {
      const { body } = stmt;
      const childEnv = new Environment(env);

      for(const newStmt of body){
        execute(newStmt, childEnv);
      }

      return;
    }
    
    case StatementKind.FunctionDeclaration: {
      const { id, params, body } = stmt;
      const fnValue = {
        kind: "function",
        params: params.map((param: Identifier) => param.symbol),
        body,
        env
      };

      env?.declareVar(id.symbol, fnValue)
      return;
    }

    case StatementKind.ReturnStatement: {
      const { argument } = stmt;
      const value = argument ? evaluate(argument, env): undefined;
      // we need to throw value so that next statements will not be executed
      throw new ReturnSignal(value); 
    }

    default: {
      throw new Error("Statement could not be processed because of unknown syntax");
    }
  }   
}

function run(ast: Program, env: Environment | null): void {
  for(const stmt of ast.body){
    execute(stmt, env); 
  }
}

const code = fs.readFileSync(path.join(__dirname, "../testFiles/test1.txt"), "utf8");  
const parser = new ProgramParser();

const ast = parser.buildAST(code);

const env = new Environment(null);
loadStdlib(env);

run(ast, env);

