// imports 
import { TokenType } from "../lexer/types";
import { ExpressionKind, Expression } from "./types";
import ParserContext from "./parserContext";

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

  private parseAssignmentExpression(): Expression {
    
    if(this.parserContext.next().type === TokenType.EQUALS){
      if(this.parserContext.peek().type !== TokenType.IDENTIFIER){
        throw new Error("Invalid syntax, cannot assign a expression to anything other than identifier");
      }

      const identifier = this.parserContext.eat().value;
      this.parserContext.eat(); // eat equals token 
      return { kind: ExpressionKind.AssignmentExpression, identifier, value: this.parseExpression() };
    }

    return this.parseLogialOrExpression(); 
  }

  private parseLogialOrExpression(): Expression {
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

  private parseLogicalAndExpression(): Expression {
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

  private parseEqualityExpression(): Expression {
    let left = this.parseComaprisonExpression();

    
    while(EQUALITY_OPERATORS.has(this.parserContext.peek().value)){
      const op = this.parserContext.eat().value;
      const right = this.parseComaprisonExpression();

      left = {
        kind: ExpressionKind.BinaryExpression,
        left,
        right,
        op,
      }
    }

    return left;
  }

  private parseComaprisonExpression(): Expression {
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

  private parseAdditiveExpression(): Expression {
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


  private parseMultiplicativeExpression(): Expression {
    
    let left = this.parsePrimaryExpression();

    while(MULTIPLICATIVE_OPERATORS.has(this.parserContext.peek().value)){
      const op = this.parserContext.eat().value;
      const right = this.parsePrimaryExpression();
      left = {
        kind: ExpressionKind.BinaryExpression,
        left,
        right, 
        op,
      }

    }

    return left; 
  }

  private parsePrimaryExpression(): Expression {
    
    switch (this.parserContext.peek().type) {
      case TokenType.NUMBER:{
        return { kind: ExpressionKind.NumericLiteral, value: parseFloat(this.parserContext.eat().value) };
      }
        
      case TokenType.IDENTIFIER: {
        return { kind: ExpressionKind.Identifier, symbol: this.parserContext.eat().value };  
      }
    
      case TokenType.OPEN_PARENTHESIS: {
        this.parserContext.eat();
        const expr = this.parseExpression();

        if(this.parserContext.eat().type !== TokenType.CLOSED_PARENTHESIS){
          console.error("Non-matching parenthesis are not allowed");
          process.exit(1);
        }

        return expr;
      }

      default: {
        console.error(`Error!, Could not identify symbol '${this.parserContext.eat().value}'`);
        process.exit(1);
      }
    }
  }

}

// priority order of operators in expression parsing  
// 1. Statement Execution
// 2. Assignment Operator 
// 3. Logical Operators
// 4. Comparison Operators 
// 4. Additive Operators 
// 5. Multiplicative Operators 
// 6. Primary expression
