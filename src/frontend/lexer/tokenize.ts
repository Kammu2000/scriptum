// imports 
import { TokenType, Token, RESERVED_WORDS } from "./types";
import { isAlpha, isAlphaNumeric, isDigit } from "./helpers";

export const tokenize = (sourceCode: string): Token[] => {
  const tokens: Token[] = [];
  const srcLen = sourceCode.length;

  let idx = 0;

  const getNumberToken = (): Token => {
    let num = "";

    while(idx < srcLen && isDigit(sourceCode[idx])){
      num += sourceCode[idx];
      idx++;
    }

    return { type: TokenType.NUMBER, value: num };
  }

  const getWordToken = (): Token => {
    let word = "";

    while(idx < srcLen && isAlphaNumeric(sourceCode[idx])){
      word += sourceCode[idx];
      idx++;
    }
    
    if(RESERVED_WORDS.has(word)){
      return { type: RESERVED_WORDS.get(word), value: word } as Token;
    }

    return { type: TokenType.IDENTIFIER, value: word };
  }

  while(idx < srcLen){ 
    switch (sourceCode[idx]) {
      case "(": {
        const token = { type: TokenType.OPEN_PARENTHESIS, value: sourceCode[idx++] };
        tokens.push(token);
        break;
      }
      
      case ")":{
        const token = { type: TokenType.CLOSED_PARENTHESIS, value: sourceCode[idx++] };
        tokens.push(token);
        break;
      } 

      case "{": {
        const token = { type: TokenType.OPEN_BRACES, value: sourceCode[idx++] };
        tokens.push(token);
        break;
      }

      case "}": {
        const token = { type: TokenType.CLOSED_BRACES, value: sourceCode[idx++] };
        tokens.push(token);
        break;
      }

      case "!": {
        if(idx + 2 < srcLen && sourceCode[idx + 1] === "=" && sourceCode[idx + 2] === "="){
          const token = { type: TokenType.BINARY_OPERATOR, value: "!==" };
          tokens.push(token);
          idx += 3;
        } 
        else {
            throw new Error("Unknown character group found starting from !");
        }

        break;
      }


      case ">": {
        if(idx + 1 < srcLen && sourceCode[idx + 1] === "="){
          const token = { type: TokenType.BINARY_OPERATOR, value: ">=" };
          tokens.push(token);
          idx += 2;
        }
        else {
          const token = { type: TokenType.BINARY_OPERATOR, value: ">" };
          tokens.push(token);
          idx += 1;
        }

        break;
      }

      case "<": {
        if(idx + 1 < srcLen && sourceCode[idx + 1] === "="){
          const token = { type: TokenType.BINARY_OPERATOR, value: "<=" };
          tokens.push(token);
          idx += 2;
        }
        else {
          const token = { type: TokenType.BINARY_OPERATOR, value: "<" };
          tokens.push(token);
          idx += 1;
        }

        break;
      }

      case "%":
      case "+":
      case "-":
      case "*":
      case "/":{
        const token = { type: TokenType.BINARY_OPERATOR, value: sourceCode[idx++] };
        tokens.push(token);
        break;
      }
     
      case ",": {
        const token = { type: TokenType.COMMA, value: sourceCode[idx++] };
        tokens.push(token);
        break;
      }

      case " ":
      case "\n":
      case "\t":{
        idx++;
        break;
      }

      case "=": {
        if(idx + 2 < srcLen && sourceCode[idx + 1] === "=" && sourceCode[idx + 1] === sourceCode[idx + 2]){
          const token = { type: TokenType.BINARY_OPERATOR, value: "==="};
          tokens.push(token);
          idx += 3;
        }
        else if(idx + 1 < srcLen && sourceCode[idx + 1] === "="){
          throw new Error("Invalid character group == found in code");
        }
        else {
          const token = { type: TokenType.EQUALS, value: "=" };
          tokens.push(token);
          idx += 1;
        }
        break;
      }

      case "&": {
        if(idx + 1 < srcLen && sourceCode[idx + 1] === "&"){
          const token = { type: TokenType.BINARY_OPERATOR, value: "&&" };
          tokens.push(token);
          idx += 2;
        } 
        else {
          throw new Error("Unknown character &");
        }

        break;
      }

      case "|": {
        if(idx + 1 < srcLen && sourceCode[idx + 1] === "|"){
          const token = { type: TokenType.BINARY_OPERATOR, value: "||" };
          tokens.push(token);
          idx += 2;
        } 
        else {
          throw new Error("Unknown character |");
       }

        break;
      }

      // multi character tokens
      default:{
        if(isDigit(sourceCode[idx])){
          const token = getNumberToken();
          tokens.push(token);
        }
        else if(isAlpha(sourceCode[idx])){
          const token = getWordToken();
          tokens.push(token);
        }
        else {
          throw new Error(`Unknown character found in code: ${sourceCode[idx]}`);
        }
      }
        break;
    }
  }

  tokens.push({ type: TokenType.EOF, value: "EOF"});
  return tokens;
}

