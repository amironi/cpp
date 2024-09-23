function findTwoSum(numbers: number[], target: number): number[] {
  let left: number = 0;
  let right: number = numbers.length - 1;

  while (left < right) {
    const currentSum: number = numbers[left] + numbers[right];
    if (currentSum === target) {
      return [left + 1, right + 1]; // Adding 1 because the array is 1-indexed
    } else if (currentSum < target) {
      left++;
    } else {
      right--;
    }
  }

  return []; // No solution found
}

// Example usage
const numbers: number[] = [2, 7, 11, 15];
const target: number = 9;

console.log(findTwoSum(numbers, target)); // Output: [1, 2]
function max_profit(prices: number[]): number {
  if (prices.length === 0) {
    return 0;
  }

  let minPrice: number = Infinity;
  let maxProfit: number = 0;

  for (const price of prices) {
    if (price < minPrice) {
      minPrice = price;
    } else if (price - minPrice > maxProfit) {
      maxProfit = price - minPrice;
    }
  }

  return maxProfit;
}

// Example usage
const prices: number[] = [7, 1, 5, 3, 6, 4];
console.log(max_profit(prices)); // Output: 5
