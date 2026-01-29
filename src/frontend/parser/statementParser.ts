// imports 
import { TokenType } from "../lexer/types";
import ExpressionParser from "./expressionParser";
import ParserContext from "./parserContext";
import { Statement, VariableDeclaration, BlockStatement, IfStatement, ExpressionStatement, StatementKind, WhileStatement, ExpressionKind } from "./types";

export default class StatementParser {
  private parserContext: ParserContext;
  private expressionParser: ExpressionParser;

  constructor(ctx: ParserContext){
      this.parserContext = ctx;
      this.expressionParser = new ExpressionParser(ctx);
  }
  
  public parseStatement(): Statement {
    switch(this.parserContext.peek().type){
      case TokenType.LET: {
        return this.parseVarDeclarationStatement();
      } 

      case TokenType.IF: {
        return this.parseIfStatement();
      }
      
      case TokenType.OPEN_BRACES: {
        return this.parseBlockStatement();
      }

      case TokenType.WHILE: {
        return this.parseWhileStatement();
      }

      default: 
        return this.parseExpressionStatement();
    }
  }
  
  private parseVarDeclarationStatement(): VariableDeclaration {
    this.parserContext.eat(); // eat let token 
    const identifier = this.parserContext.eat().value;
    let value;

    if(this.parserContext.peek().type === TokenType.EQUALS){
      this.parserContext.eat(); // eat equals token
      value = this.expressionParser.parseExpression();
    }

    return { kind: StatementKind.VariableDeclaration, identifier: identifier, value };      
  }

  private parseIfStatement(): IfStatement {
    this.parserContext.eat(); // eat if token

    if(this.parserContext.eat().type !== TokenType.OPEN_PARENTHESIS){
      throw new Error("Expected open parenthesis while starting condition of if statement");
    }

    const condition = this.expressionParser.parseExpression();

    if(this.parserContext.eat().type !== TokenType.CLOSED_PARENTHESIS){
      throw new Error("Expected closed parenthesis while ending condition of if statement");
    }
    
    if(this.parserContext.peek().type !== TokenType.OPEN_BRACES){
      throw new Error("No open braces found at staring of if statement");
    }

    const thenBlock = this.parseBlockStatement(); 
    let elseBlock;
    
    if(this.parserContext.peek().type === TokenType.ELSE){
      this.parserContext.eat();
      if(this.parserContext.peek().type === TokenType.IF){
        elseBlock = this.parseIfStatement();
      }
      else if(this.parserContext.peek().type === TokenType.OPEN_BRACES){
        elseBlock = this.parseBlockStatement();
      }
      else {
        throw new Error("else statement should have block or a new if statement");
      }
    }

    return { kind: StatementKind.IfStatement, condition, thenBlock, elseBlock };
  }


  private parseBlockStatement(): BlockStatement {
    this.parserContext.eat();

    const body: Statement[] = [];

    while(!this.parserContext.endOf() && this.parserContext.peek().type !== TokenType.CLOSED_BRACES){
      const stmt = this.parseStatement();
      body.push(stmt);
    }
    
    if(this.parserContext.peek().type !== TokenType.CLOSED_BRACES){
      throw new Error("No matching closed braces for open braces");
    }
    else {
      this.parserContext.eat();
    }
    
    return { kind: StatementKind.BlockStatement, body };
  }

  private parseWhileStatement(): WhileStatement{
    this.parserContext.eat();

    if(this.parserContext.eat().value !== "("){
      throw new Error("No open parenthesis was found in while loop starting");
    }
    
    const expr = this.expressionParser.parseExpression();
    
    if(this.parserContext.eat().value !== ")"){
      throw new Error("No matching closed parenthesis was found for open parenthesis");
    }
    
    if(this.parserContext.eat().value !== "{"){
      throw new Error("No open braces in while loop body");
    }

    const loopbody = this.parseBlockStatement();

    return { kind: StatementKind.WhileStatement, condition: expr, body: loopbody };
  }

private parseExpressionStatement(): ExpressionStatement {
    const expr = this.expressionParser.parseExpression();
    return { kind: StatementKind.ExpressionStatement, expression: expr };
  }
}

