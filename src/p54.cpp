/**
 * High Card:       Highest value card.
 * One Pair:        Two cards of the same value.
 * Two Pairs:       Two different pairs.
 * Three of a Kind: Three cards of the same value.
 * Straight:        All cards are consecutive values.
 * Flush:           All cards of the same suit.
 * Full House:      Three of a kind and a pair.
 * Four of a Kind:  Four cards of the same value.
 * Straight Flush:  All cards are consecutive values of same suit.
 * Royal Flush:     Ten, Jack, Queen, King, Ace, in same suit.
 *
 * The cards are valued in the order:
 * 2, 3, 4, 5, 6, 7, 8, 9, 10, Jack, Queen, King, Ace.
 *
 * If two players have the same ranked hands then the rank made up
 * of the highest value wins; for example, a pair of eights beats
 * a pair of fives (see example 1 below). But if two ranks tie,
 * for example, both players have a pair of queens, then highest cards
 * in each hand are compared (see example 4 below); if the highest cards
 * tie then the next highest cards are compared, and so on.
 */

#include <iostream>
#include <algorithm>
#include "euler.h"

BEGIN_PROBLEM(54, solve_problem_54)
    PROBLEM_TITLE("How many hands did player one win")
    PROBLEM_ANSWER("376")
    PROBLEM_DIFFICULTY(1)
    PROBLEM_FUN_LEVEL(1)
    PROBLEM_TIME_COMPLEXITY("N")
    PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

static const char value_s[] = { '2','3','4','5','6','7','8','9','T','J','Q','K','A' };
static const char suit_s[] = { 'S','H','D','C' };

struct card_t
{
    unsigned char value; // corresponds to value_s
    unsigned char suit;  // corresponds to suit_s
};

enum rank_t
{
    HighCard,      // 1+1+1+1+1
    OnePair,       // 2+1+1+1
    TwoPairs,      // 2+2+1
    ThreeOfAKind,  // 3+1+1
    Straight,
    Flush,
    FullHouse,     // 3+2
    FourOfAKind,   // 4+1
    StraightFlush,
    RoyalFlush,
};

static rank_t reorder_hand(card_t h[5])
{
    static int index[] = { 0, 1, 2, 3, 4 };

    // sort the cards by kind (i.e. 3+2)
    unsigned char count[13] = { 0 };
    std::for_each(h + 0, h + 5, [&count](card_t c) {
        ++count[c.value];
    });
    std::sort(h + 0, h + 5, [&count](card_t a, card_t b) {
        return (count[a.value] > count[b.value]) ||
            (count[a.value] == count[b.value] && a.value > b.value);
    });

    // check whether the hand is a flush or straight
    bool is_flush = std::all_of(h + 1, h + 5, [h](card_t c) {
        return c.suit == h[0].suit;
    });
    bool is_straight = std::all_of(index + 1, index + 5, [h](int i) {
        return h[i].value == h[0].value - i;
    });

    if (is_flush && h[0].value == 12)
    {
        return RoyalFlush;
    }
    if (is_flush && is_straight)
    {
        return StraightFlush;
    }
    if (h[0].value == h[1].value && h[1].value == h[2].value &&
        h[2].value == h[3].value)
    {
        return FourOfAKind;
    }
    if (h[0].value == h[1].value && h[1].value == h[2].value &&
        h[3].value == h[4].value)
    {
        return FullHouse;
    }
    if (is_flush)
    {
        return Flush;
    }
    if (is_straight)
    {
        return Straight;
    }
    if (h[0].value == h[1].value && h[1].value == h[2].value)
    {
        return ThreeOfAKind;
    }
    if (h[0].value == h[1].value && h[2].value == h[3].value)
    {
        return TwoPairs;
    }
    if (h[0].value == h[1].value)
    {
        return OnePair;
    }
    return HighCard;
}

static int compare_hands(card_t a[5], card_t b[5])
{
    rank_t r1 = reorder_hand(a);
    rank_t r2 = reorder_hand(b);
    if (r1 < r2)
    {
        return -1;
    }
    if (r1 > r2)
    {
        return 1;
    }
    for (int i = 0; i < 5; i++)
    {
        if (a[i].value < b[i].value)
        {
            return -1;
        }
        if (a[i].value > b[i].value)
        {
            return 1;
        }
    }
    return 0;
}

static card_t read_card(char value, char suit)
{
    card_t c;
    c.value = (unsigned char)(std::find(value_s + 0, value_s + sizeof(value_s), value) - value_s);
    c.suit = (unsigned char)(std::find(suit_s + 0, suit_s + sizeof(suit_s), suit) - suit_s);
    return c;
}

static void solve_problem_54()
{
    int count = 0;
    const char *input[] = {
#include "p54-poker.txt"
    };
    const int num_rounds = sizeof(input) / sizeof(input[0]) / 10;
    for (int round = 0; round < num_rounds; round++)
    {
        card_t cards[10];
        for (int i = 0; i < 10; i++)
        {
            cards[i] = read_card(
                input[round * 10 + i][0],
                input[round * 10 + i][1]);
        }

        int winner = compare_hands(&cards[0], &cards[5]);
#if 0
        std::cout << row << "   ";
        if (winner > 0)
            std::cout << "Player 1 wins" << std::endl;
        else if (winner < 0)
            std::cout << "Player 2 wins" << std::endl;
        else
            std::cout << "Tie";
#endif
        if (winner > 0)
            ++count;
    }
    std::cout << count << std::endl;
}
