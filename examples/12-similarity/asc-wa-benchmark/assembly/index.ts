const array: i32[] = [];
const charCodeCache: i32[] = [];

export function leven(left: String, right: String): i32 {
  if (left.length > right.length) {
    const swap = right;
    right = left;
    left = swap;
  }

  let leftLength: i32 = left.length - 1;
  let rightLength: i32 = right.length - 1;
  while (
    leftLength > 0 &&
    left.charCodeAt(leftLength) === right.charCodeAt(rightLength)
  ) {
    leftLength -= 1;
    rightLength -= 1;
  }
  leftLength += 1;
  rightLength += 1;
  let start: i32 = 0;
  while (
    start < leftLength &&
    left.charCodeAt(start) === right.charCodeAt(start)
  ) {
    start += 1;
  }
  leftLength -= start;
  rightLength -= start;
  if (leftLength === 0) {
    return rightLength;
  }
  for (let i = 0; i < leftLength; i += 1) {
    charCodeCache[i] = left.charCodeAt(start + i);
    array[i] = i + 1;
  }
  let bCharCode: i32;
  let result: i32 = 0;
  let temp: i32;
  let temp2: i32;
  let j: i32 = 0;
  while (j < rightLength) {
    bCharCode = right.charCodeAt(start + j);
    temp = j;
    j += 1;
    result = j;
    for (let i = 0; i < leftLength; i += 1) {
      /* eslint-disable */
      temp2 = temp + (bCharCode !== charCodeCache[i] ? 1 : 0);
      /* eslint-enable */
      temp = array[i];
      if (temp > result) {
        array[i] = temp2 > result ? result + 1 : temp2;
      } else {
        array[i] = temp2 > temp ? temp + 1 : temp2;
      }
      result = array[i];
    }
  }
  return result;
}