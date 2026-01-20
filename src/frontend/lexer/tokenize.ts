// imports 
import { TokenType, Token, reserveWords } from "./types";
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
    
    if(reserveWords.has(word)){
      return { type: reserveWords.get(word), value: word } as Token;
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

      case "+":
      case "-":
      case "*":
      case "/":{
        const token = { type: TokenType.BINARY_OPERATOR, value: sourceCode[idx++] };
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
        const token = { type: TokenType.EQUALS, value: sourceCode[idx++] };
        tokens.push(token);
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

const code = "let temp = 5 * (3 + 4)";
console.log(tokenize(code));

