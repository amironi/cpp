function makeChange(N, coins) {
    // initialize the 'ways' array
    var answers = [];
    for (var n = 1; n <= N; n++) {
        answers[n] = 0;
    }
    // there is only one way of doing 0 cents?
    // this is initialized for the first pass through of the problem
    answers[0] = 1;
        
    // go through the coins one at a time
    coins.forEach(function(coin){
        // test each value after you are able to use at least one of the coin
        for (var i = coin; i <= N; i++) {
            // can you make change from the remainder
            // after subtracting out another coin?
            var rem = i - coin;
            answers[i] += answers[rem];
        }
    });
    // answers at the amount will be the sum of all possibilities
    return answers[N];
    
} 