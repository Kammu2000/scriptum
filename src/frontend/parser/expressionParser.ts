// imports 
import { TokenType } from "../lexer/types";
import { ExpressionKind, Expression, NumericLiteral, Identifier } from "./types";
import ParserContext from "./parserContext";

const ASSIGNMENT_OPERATORS = new Set(["=", "+=", "-=", "*=", "/="]);
const EQUALITY_OPERATORS = new Set(["===", "!=="]);
const COMPARISON_OPERATORS = new Set([">", "<", ">=", "<="]);
const ADDITIVE_OPERATORS = new Set(["+", "-"]);
const MULTIPLICATIVE_OPERATORS = new Set(["*", "/", "%"]);

export default class ExpressionParser {
  private parserContext: ParserContext;

  constructor(ctx: ParserContext){
    this.parserContext = ctx;
  }

  public parseExpression(): Expression {
    return this.parseAssignmentExpression();
  }

  public parseAssignmentExpression(): Expression {
    const left = this.parseLogicalOrExpression();

    if(ASSIGNMENT_OPERATORS.has(this.parserContext.peek().value)){
      // TODO: Later left side can be an assignable expression like member expression obj.x (identifier will work now)

      if(left.kind !== ExpressionKind.Identifier){
        throw new Error("Invalid syntax, cannot assign a expression to anything other than identifier");
      }

      const op = this.parserContext.eat().value; // eat assignemnt token 
      return { kind: ExpressionKind.AssignmentExpression, left, right: this.parseAssignmentExpression(), op };
    }

    return left;
  }

  public parseLogicalOrExpression(): Expression {
    let left = this.parseLogicalAndExpression();

    
    while(this.parserContext.peek().value === "||"){
      const op = this.parserContext.eat().value;
      const right = this.parseLogicalAndExpression();

      left = {
        kind: ExpressionKind.BinaryExpression,
        left,
        right,
        op,
      }
    }

    return left;
  }

  public parseLogicalAndExpression(): Expression {
    let left = this.parseEqualityExpression();

    
    while(this.parserContext.peek().value === "&&"){
      const op = this.parserContext.eat().value;
      const right = this.parseEqualityExpression();

      left = {
        kind: ExpressionKind.BinaryExpression,
        left,
        right,
        op,
      }
    }

    return left;
  }

  public parseEqualityExpression(): Expression {
    let left = this.parseComparisonExpression();

    
    while(EQUALITY_OPERATORS.has(this.parserContext.peek().value)){
      const op = this.parserContext.eat().value;
      const right = this.parseComparisonExpression();

      left = {
        kind: ExpressionKind.BinaryExpression,
        left,
        right,
        op,
      }
    }

    return left;
  }

  public parseComparisonExpression(): Expression {
    let left = this.parseAdditiveExpression();

    
    while(COMPARISON_OPERATORS.has(this.parserContext.peek().value)){
      const op = this.parserContext.eat().value;
      const right = this.parseAdditiveExpression();

      left = {
        kind: ExpressionKind.BinaryExpression,
        left,
        right,
        op,
      }
    }

    return left;
  }

  public parseAdditiveExpression(): Expression {
    let left = this.parseMultiplicativeExpression();

    while(ADDITIVE_OPERATORS.has(this.parserContext.peek().value)){
      const op = this.parserContext.eat().value;
      const right = this.parseMultiplicativeExpression();

      left = {
        kind: ExpressionKind.BinaryExpression,
        left,
        right, 
        op,
      }
    }

    return left;
  }


  public parseMultiplicativeExpression(): Expression {
    let left = this.parseCallExpression();

    while(MULTIPLICATIVE_OPERATORS.has(this.parserContext.peek().value)){
      const op = this.parserContext.eat().value;
      const right = this.parseCallExpression();
      left = {
        kind: ExpressionKind.BinaryExpression,
        left,
        right, 
        op,
      }

    }

    return left; 
  }

  public parseCallExpression(): Expression {
    let expr = this.parsePrimaryExpression();

    while(this.parserContext.peek().type === TokenType.OPEN_PARENTHESIS){
      this.parserContext.eat(); // eat '(' 

      const args = this.parseArguments();
        expr = {
        kind: ExpressionKind.CallExpression,
        callee: expr,
        args
      }

      if(this.parserContext.peek().type === TokenType.CLOSED_PARENTHESIS){
        this.parserContext.eat();
      }
      else {
        throw new Error("No matching closed parenthesis");
      }
    }

    return expr;
  }

  public parsePrimaryExpression(): Expression {
    
    switch (this.parserContext.peek().type) {
      case TokenType.NUMBER:{
        return this.parseNumericLiteral();
      }
        
      case TokenType.IDENTIFIER: {
        return this.parseIdentifier();  
      }
    
      case TokenType.OPEN_PARENTHESIS: {
        return this.parseGroupedExpression();
      }

      default: {
        console.error(`Error!, Could not identify symbol '${this.parserContext.eat().value}'`);
        process.exit(1);
      }
    }
  }

  public parseIdentifier(): Identifier {
    if (this.parserContext.peek().type !== TokenType.IDENTIFIER) {
      throw new Error("Expected identifier");
    }

    return {
      kind: ExpressionKind.Identifier,
      symbol: this.parserContext.eat().value,
    };
  }

  public parseNumericLiteral(): NumericLiteral {
    if(this.parserContext.peek().type !== TokenType.NUMBER){
      throw new Error("Expected number");
    }

    return {
      kind: ExpressionKind.NumericLiteral,
      value: parseFloat(this.parserContext.eat().value),
    };
  }

  public parseGroupedExpression(): Expression {
    if(this.parserContext.peek().type !== TokenType.OPEN_PARENTHESIS){
      throw new Error("Expected open parenthesis");
    }

    this.parserContext.eat(); // '('
    const expr = this.parseExpression();

    if (this.parserContext.peek().type !== TokenType.CLOSED_PARENTHESIS) {
      throw new Error("Non-matching parenthesis");
    }

    this.parserContext.eat(); // ')'
    return expr;
  }

  public parseArguments(): Expression[] {
    const args: Expression[] = [];

    if(this.parserContext.peek().type === TokenType.CLOSED_PARENTHESIS){
      return args;
    }

    args.push(this.parseExpression());

    while(!this.parserContext.endOf() && this.parserContext.peek().type === TokenType.COMMA){
      this.parserContext.eat(); // eat comma 
      args.push(this.parseExpression());
    }
    
    if(this.parserContext.peek().type !== TokenType.CLOSED_PARENTHESIS){
      throw new Error("No matching closed parenthesis")
    }
    
    return args;
  } 
}

// priority order of operators in expression parsing  
// 1. Statement Execution
// 2. Assignment Operator 
// 3. Logical Operators
// 4. Comparison Operators 
// 5. Additive Operators 
// 6. Multiplicative Operators 
// 7. Call Expression 
// 8. Primary expression
