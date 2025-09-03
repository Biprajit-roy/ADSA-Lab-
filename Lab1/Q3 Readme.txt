Q3. Divide and Conquer algorithm to find the lighter (defective) coin

First, split the coins into two groups of equal size; if n is odd, leave one coin x aside.
Weigh the two equal groups on the balance.

If the scale balances, then no coin in those groups is lighter; the only possibilities are: (i) the aside coin x is lighter, or (ii) no coin is lighter. Compare x with any coin from the balanced groups to decide:
	x lighter ⇒ return x.
	equal ⇒ return “none”.

If the scale tilts, the lighter pan contains the defective coin.  Then recurse on that group.

This gives the recurrence T(n) = T(⌈n/2⌉) + 1.
Hence, T(n) = ⌈log₂ n⌉ + c ,for some constant c.
