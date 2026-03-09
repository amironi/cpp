int max_profit(const vector<int>& stock)
{
  int min_stock = stock[0]; 

  int max_profit = stock[1];
  
  for( int i = 2; i < stock.size(); i++ )
  {
    max_profit = max(max_profit, stock[i] - min_stock);

    min_stock = min(min_stock, stock[i]);
  }
  
  return max_profit;

}

// Returns maximum profit with two transactions on a given
// list of stock prices, price[0..n-1]
int maxProfit(int price[], int n)
{
    // Create profit array and initialize it as 0
    int *profit = new int[n];
    for (int i=0; i<n; i++)
        profit[i] = 0;
 
    /* Get the maximum profit with only one transaction
       allowed. After this loop, profit[i] contains maximum
       profit from price[i..n-1] using at most one trans. */
    int max_price = price[n-1];
    for (int i=n-2;i>=0;i--)
    {
        // max_price has maximum of price[i..n-1]
        max_price = max( max_price, price[i]);
 
        // we can get profit[i] by taking maximum of:
        // a) previous maximum, i.e., profit[i+1]
        // b) profit by buying at price[i] and selling at
        //    max_price
        profit[i] = max(profit[i+1], max_price-price[i]);
    }
 
    /* Get the maximum profit with two transactions allowed
       After this loop, profit[n-1] contains the result */
    int min_price = price[0];
    for (int i=1; i<n; i++)
    {
        // min_price is minimum price in price[0..i]
        
        min_price = min( min_price, price[i]);
        // Maximum profit is maximum of:
        // a) previous maximum, i.e., profit[i-1]
        // b) (Buy, Sell) at (min_price, price[i]) and add
        //    profit of other trans. stored in profit[i]
        profit[i] = max(profit[i-1], profit[i] +
                                    (price[i]-min_price) );
    }
    int result = profit[n-1];
 
    delete [] profit; // To avoid memory leak
 
    return result;
}


/*
def get_max_profit(stock_prices_yesterday):

    if len(stock_prices_yesterday) < 2:
        raise IndexError('Getting a profit requires at least 2 prices')

    min_price = stock_prices_yesterday[0]
    max_profit = stock_prices_yesterday[1] - stock_prices_yesterday[0]

    for index, current_price in enumerate(stock_prices_yesterday):

        if index == 0:
            continue

        potential_profit = 

        max_profit = max(max_profit, current_price - min_price)

        min_price  = min(min_price, current_price)

    return max_profit




*/