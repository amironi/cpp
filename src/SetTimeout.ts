// Virtual timer

// physSetTimeout(millSec, callback)

// example 1
// after 1sec -> 'chen'
SetTimeout(1000, console.log("chen"));

// example 2
// 3 secs -> 'amir'
// now = 0
SetTimeout(1000, console.log("chen")); // 0
SetTimeout(3000, console.log("amir")); // 0

// example 3
// time = 0
SetTimeout(1000, console.log("chen"));
// time = 2000
SetTimeout(2000, console.log("amir"));

// example 4
// time = 0
SetTimeout(7000, console.log("chen"));
// time = 2000
SetTimeout(2000, console.log("amir"));

// time = 4
// 'amir'
// x = 4
// time = 7
// 'chen'

// time = 1 100-> 'chen'
// time = 3 -> 'amir'

function insertSorted(list: item[], newItem: item): void {
  const key = (i: item) => i.time;
  const index = list.findIndex((i) => key(i) > key(newItem));

  if (index === -1) {
    list.push(newItem);
  } else {
    list.splice(index, 0, newItem);
  }
}

interface item {
  callback: () => void;
  // now: number;
  abs_time: number;
}

let list: item[] = []; // ordered by now+millsecond?

function callback_(item: item) {
  item.callback();

  if (list.length > 0) {
    list = list.slice(0);

    setTimeout(item.abs_time - Date.now(), () => callback_(list[0]));
  }
}

function setTimeout2(millsecond: number, callback: () => void) {
  const abs_time = Date.now() + millsecond;
  list.push({ callback, abs_time });

  setTimeout(list[0].abs_time - Date.now(), () => callback_(list[0]));
}
