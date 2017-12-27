# CoinChain

Unlike current cryptocurrencies (such as BitCoin), CoinChain isn't a single currency, it is an entire set of currencies.
For each user there is a currency in the set, and each unit of each such currency has its unique blockchain, which tracks who currently owns said unit.

Another major difference: A CoinChain currency isn't mined, instead the user who's existence spawned the currency is the only entity capable of creating units of said currency.
How you may ask? By creating the "anchor" of the blockchain tracking the owner of the unit of the currency.
In effect, each user can print as many units of the CoinChain currency their existence spawned as the number of bits in the anchor's UID allows. However, they cannot create any unit of any other CoinChain currency.
As a result, each CoinChain currency is as stable as the user that spawned it wishes.

# Blockchain

In short, the blockchain begins with an [Anchor], followed by any number of [FullLink]s, potentially ending in a single [HalfLink]. (although [HalfLink]s don't need to be tracked by anyone other than [new owner] and [previous owner])
It is important to note that each <SIGNATURE> verifies the ENTIRE previous blockchain, including any previous parts of the current block.
As such, the order of fields listed below in relation to the <SIGNATURE>s is important.

[Anchor] :
	<PUB_KEY> of [issuer]
	<UID> of this unit of currency
	<SIGNATURE> of issuer, ensuring validity of [Anchor]
	
[FullLink] :
	<PUB_KEY> of [new owner]
	<SIGNATURE> of [previous owner], ensuring the transaction isn't a theft
	<SIGNATURE> of [new owner], signifying the transaction to be complete

[HalfLink] :
	<PUB_KEY> of [new owner]
	<SIGNATURE> of [previous owner], see [Link]

# Conflict resolution

[HalfLink]s, don't conflict with one another, as they aren't completed transactions.
If a [HalfLink] conflicts with a [FullLink], the [FullLink] overrides the [HalfLink].
If two [FullLink]s conflict, the [FullLink] that become known beforehand overrides the other.

# This Library

This Library is merely a simple C implementation to help elaborate the concept of CoinChain.