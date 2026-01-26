// imports 
import Parser from "../frontend/parser/ast";
import { Expression, ExpressionKind, Program, Statement, StatementKind } from "../frontend/parser/types";
import Environment from "./environment";

function evaluate(expr: Expression, env: Environment | null): any {

  switch (expr.kind) {
    case ExpressionKind.NumericLiteral:
      return expr.value;  
    
    case ExpressionKind.Identifier: 
      return env?.lookupVar(expr.symbol);

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
       
        default: {
          console.error(`Could not identify the operator ${expr.op}`);
          process.exit(1);
        }
      }
    }


    default:
      break;
  }
  
}

function execute(stmt: Statement, env: Environment | null): void {
  switch (stmt.kind) {
    case StatementKind.ExpressionStatement: {
      const { expression } = stmt;
      const val = evaluate(expression, env); 
      console.log("deepanshu", val);
      return;
    }
      
    default:
      break;
  }   
}

function run(ast: Program, env: Environment | null): void {
  for(const stmt of ast.body){
    execute(stmt, env); 
  }
}

// console.log(JSON.stringify(ast));

const code = "(x * (6 - 7)) + (3 + 4)"
const parser = new Parser();

const ast = parser.buildAST(code);
const env = new Environment(null);
env.declareVar("x", 5);

run(ast, env);
