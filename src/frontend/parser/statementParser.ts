// imports 
import { TokenType } from "../lexer/types";
import ExpressionParser from "./expressionParser";
import ParserContext from "./parserContext";
import { Statement, VariableDeclaration, BlockStatement, IfStatement, ExpressionStatement, StatementKind, WhileStatement, ExpressionKind, ReturnStatement, FunctionDeclaration, Identifier } from "./types";

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
        return this.parseVarDeclaration();
      } 

      case TokenType.IF: {
        return this.parseIfStatement();
      }
      
      case TokenType.WHILE: {
        return this.parseWhileStatement();
      }

      case TokenType.OPEN_BRACES: {
        return this.parseBlockStatement();
      }

      case TokenType.RETURN: {
        return this.parseReturnStatement();
      }

      case TokenType.FUNCTION: {
        return this.parseFunctionDeclaration();
      }

      default: 
        return this.parseExpressionStatement();
    }
  }
  
  private parseVarDeclaration(): VariableDeclaration {
    this.parserContext.eat(); // eat let token 

    if(this.parserContext.peek().type !== TokenType.IDENTIFIER){
      throw new Error("Variable declaration should contain identifier after let");
    }
    
    const identifier = this.expressionParser.parseIdentifier();
    let init;

    if(this.parserContext.peek().type === TokenType.EQUALS){
      this.parserContext.eat(); // eat equals token
      init = this.expressionParser.parseExpression();
    }

    return { kind: StatementKind.VariableDeclaration, id: identifier, init };      
  }

  private parseIfStatement(): IfStatement {
    this.parserContext.eat(); // eat if token

    const test = this.expressionParser.parseGroupedExpression(); 
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

    return { kind: StatementKind.IfStatement, test, thenBlock, elseBlock };
  }

  private parseBlockStatement(): BlockStatement {
    if(this.parserContext.peek().type !== TokenType.OPEN_BRACES){
      throw new Error("No open braces found at staring of in block statement");
    }
    else { 
      this.parserContext.eat();
    }

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
    const test = this.expressionParser.parseGroupedExpression();
    const loopbody = this.parseBlockStatement();

    return { kind: StatementKind.WhileStatement, test, body: loopbody };
  }

  private parseReturnStatement(): ReturnStatement {
    this.parserContext.eat();

    if(this.parserContext.peek().type === TokenType.CLOSED_BRACES){
        return { kind: StatementKind.ReturnStatement };
    }

    return { kind: StatementKind.ReturnStatement, argument: this.expressionParser.parseExpression() };
  }

  private parseFunctionDeclaration(): FunctionDeclaration {
    this.parserContext.eat(); // eat function word

    if(this.parserContext.peek().type !== TokenType.IDENTIFIER) {
      throw new Error("Anonymous functions are not allowed in this language");
    }

    const id = this.expressionParser.parseIdentifier(); 

    if(this.parserContext.peek().type !== TokenType.OPEN_PARENTHESIS){
      throw new Error("function definition is not correct, no parenthesis after name");
    }
    else this.parserContext.eat();

    const params: Identifier[] = [];

    while(this.parserContext.peek().type !== TokenType.CLOSED_PARENTHESIS){
      if(this.parserContext.peek().type !== TokenType.IDENTIFIER){
        throw new Error("function parameter is not an identifier");
      }

      params.push(this.expressionParser.parseIdentifier());
      
      if(this.parserContext.peek().type === TokenType.COMMA){
        if(this.parserContext.next().type === TokenType.CLOSED_PARENTHESIS){
          throw new Error("Trailing comma is not allowed after all function parameters");
        }

        this.parserContext.eat();
      }
      else {
        break;
      }
    }

    if(this.parserContext.eat().type !== TokenType.CLOSED_PARENTHESIS){
      throw new Error("No matching closed parenthesis was found in function declaration");
    }

    const body = this.parseBlockStatement();

    return { kind: StatementKind.FunctionDeclaration , id, params, body};
  }
  
  private parseExpressionStatement(): ExpressionStatement {
    const expr = this.expressionParser.parseExpression();
    return { kind: StatementKind.ExpressionStatement, expression: expr };
  }
}

