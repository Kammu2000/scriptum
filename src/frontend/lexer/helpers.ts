export const isDigit = (ch: string): boolean => {
   return ch >= "0" && ch <= "9"; 
}

export const isAlpha = (ch: string): boolean => {
  return (ch >= "a" && ch <= "z") || (ch >= "A" && ch <= "Z") || ch === "_";
}

export const isAlphaNumeric = (ch: string): boolean => isAlpha(ch) || isDigit(ch);

