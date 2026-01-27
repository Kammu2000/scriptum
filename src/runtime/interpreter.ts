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

    case ExpressionKind.AssignmentExpression: {
      const { identifier, value } = expr;

      const computedValue = evaluate(value, env);
      env?.assignVar(identifier, computedValue); 

      return computedValue; 
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
      return;
    }

    case StatementKind.VariableDeclaration: {
      const { identifier, value } = stmt;
      const val = value ? evaluate(value, env): value;
      env?.declareVar(identifier, val);
      return; 
    }
      
    case StatementKind.IfStatement: {
      const { condition, thenBlock, elseBlock } = stmt;
      
      if(evaluate(condition, env)){
        execute(thenBlock, env);
      }
      else {
        if(elseBlock)
          execute(elseBlock, env);
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

const code = "let x = 10 if(0) { let y = 10 x = y + (x * 8) } else if(x) { x = 0 } ";
const parser = new Parser();

const ast = parser.buildAST(code);
const env = new Environment(null);

run(ast, env);

console.log(env);
