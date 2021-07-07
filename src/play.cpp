#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include "game.hpp"

extern const uint16_t BOARD_WIDTH;
extern const uint16_t BOARD_HEIGHT;
extern const uint16_t BOARD_SIZE;

uint16_t get_move_input(HexState state) {
    std::string input;
    while (true) {
        std::cout << (state.currentPlayer == Player::Black ? "B" : "W") << " => ";
        getline(std::cin, input);
        if (input.length() < 2) {
            std::cout << "invalid" << std::endl;
            continue;
        }
        char letter = input.at(0);
        uint16_t number;
        try {
            number = std::stoi(input.substr(1));
        } catch (...) {
            std::cout << "invalid" << std::endl;
            continue;
        }
        uint16_t x, y;
        if (letter >= 'a' && letter < ('a' + BOARD_WIDTH)) {
            x = letter - 'a';
        } else {
            std::cout << "invalid" << std::endl;
            continue;
        }
        if (number >= 1 && number <= BOARD_HEIGHT) {
            y = number - 1;
        } else {
            std::cout << "invalid" << std::endl;
            continue;
        }
        uint16_t pos = IX(x, y);
        if (state.board[pos] == CellState::Empty) {
            return pos;
        } else {
            std::cout << "invalid" << std::endl;
            continue;
        }
    }
}

void bench() {
    HexBoard b = HexBoard();
    // uint16_t moves[] = {
    //     695,  294,  92,  641,  436,  403,  186,  10,   680, 198,  413,  967,  748, 501,  34, 952,
    //     516,  443,  922, 878,  568,  384,  248,  589,  997, 399,  174,  445,  782, 669,  196,
    //     744, 194,  131,  241, 26,   957,  400,  730,  456,  169, 311,  2,    357,  928, 486, 144,
    //     422, 77,   991,  475, 668,  136,  49,   279,  202,  253, 312,  720,  37,   417, 8,    70,
    //     846, 659,  493,  520, 506,  590,  894,  877,  736,  206, 431,  666,  333,  297, 421, 460,
    //     999, 544,  749,  9,   32,   578,  920,  215,  176,  104, 423,  243,  457,  477, 734, 411,
    //     272, 733,  836,  737, 395,  115,  750,  451,  761,  823, 337,  389,  929,  937, 621, 135,
    //     815, 201,  841,  729, 642,  717,  1010, 944,  511,  970, 708,  242,  267,  214, 420,  57,
    //     394, 487,  31,   956, 541,  847,  671,  760,  614,  117, 466,  883,  643,  638, 79, 688,
    //     314, 697,  740,  64,  344,  228,  494,  912,  828,  444, 913,  465,  219,  35,  857, 649,
    //     566, 232,  825,  865, 146,  898,  125,  325,  874,  839, 667,  665,  416,  617, 935, 939,
    //     44, 632,  162,  860, 45,   76,   240,  85,   534,  469, 301,  623,  904,  244, 691,  670,
    //     796, 795,  530,  813, 18,   679,  587,  210,  1,    584, 107,  593,  941,  731, 792, 346,
    //     925, 224,  885,  246, 100,  62,   113,  345,  700,  473, 943,  132,  396,  265, 36, 191,
    //     305, 694,  307,  635, 230,  197,  976,  685,  693,  205, 418,  122,  46,   735, 706, 193,
    //     533, 81,   264,  684, 208,  73,   226,  751,  390,  988, 164,  0,    753,  772, 824, 790,
    //     765, 48,   827,  763, 298,  871,  28,   1000, 498,  719, 752,  341,  971,  770, 996, 634,
    //     739, 981,  355,  985, 180,  263,  317,  96,   19,   837, 810,  163,  622,  538, 631,
    //     1011, 908, 546,  87,   71,  859,  13,   338,  773,  309,  479, 681,  1019, 965,  143,
    //     876,  375,  856, 742,  993,  276, 936,  629,  236,  52,   376,  489, 869,  315,  342,
    //     598, 791,  575,  594, 225,  826,  801, 863,  25,   664,  949,  306,  192, 677,  40, 481,
    //     461, 458,  780,  145, 361,  787,  597, 22,   745,  287,  651,  933,  537, 292,  209,
    //     1004, 817, 220,  812,  329, 551,  814,  974, 65,   482,  543,  536,  663,  4,   347, 353,
    //     352,  356, 781,  15,   83, 650,  1012, 961, 351,  625,  702,  807,  627,  462, 490,  471,
    //     599,  173, 689,  959,  78, 207,  783,  889, 67,   218,  984,  463,  133,  580, 14,   233,
    //     747,  653, 872,  525,  138, 322,  238,  890, 565,  891,  179,  304,  480,  676, 698, 185,
    //     804,  880, 369,  3,    12, 50,   478,  794, 229,  291,  966,  467,  800,  175, 1021, 74,
    //     425,  161, 504,  926,  613, 539,  633,  570, 909,  190,  524,  601,  449,  934, 373, 270,
    //     906,  170, 111,  336,  437, 916,  918,  366, 367,  358,  1015, 948,  401,  562, 320, 497,
    //     526,  21,  95,   696,  896, 692,  574,  921, 779,  33,   343,  1017, 140,  476, 899, 558,
    //     986,  655, 723,  712,  448, 155,  269,  428, 332,  703,  548,  172,  581,  188, 316, 987,
    //     789,  517, 349,  129,  274, 157,  519,  447, 803,  975,  434,  108,  832,  573, 884,
    //     1023, 1016, 554, 754,  640,  41, 360,  710,  690, 858,  816,  555,  20,   523,  382, 23,
    //     500,  324,  391, 184,  211,  204, 464,  440,  612, 17,   609,  522,  821,  758,  88, 372,
    //     491,  433,  637, 408,  110,  273, 348,  171,  359, 152,  610,  290,  549,  446,  414,
    //     674,  528,  886,  130, 879,  769,  586, 998,  354,  822, 30,   60,   388,  160,  323,
    //     778, 256,  785,  275,  833, 756,  808,  592, 867,  802,  38,  850,  123,  339,  470, 319,
    //     777, 199,  881,  66,   755, 227,  542,  488, 105,  55,   251, 495,  595,  75,   260,  59,
    //     607, 600,  990,  165,  153, 738,  116,  178, 282,  5,    124, 910,  455,  310,  189,
    //     1018, 834, 605,  286,  426,  441, 300,  861,  268, 766,  776,  432, 405,  757,  572, 915,
    //     532,  656, 983,  875,  383,  960, 806,  797,  759, 1008, 261,  177, 106,  381,  660, 644,
    //     624,  103, 979,  365,  647,  502, 392,  410,  798, 661,  84,   852, 529,  982,  468, 654,
    //     1013, 159, 429,  911,  293,  430, 954,  239,  313, 284,  784,  158, 788,  249,  707, 182,
    //     398,  250, 657,  764,  799,  602, 564,  51,   318, 1009, 212,  195, 43,   648,  930, 370,
    //     453,  412, 855,  848,  953,  141, 266,  148,  1007, 119,  258,  902, 86,   583,  917,
    //     714,  485,  29,  255,  402,  109,  328, 687,  905,  484, 438,  992,  923, 1001, 121, 973,
    //     854,  818,  427, 156,  866,  379,  713, 69,   845,  853, 716,  221,  454, 407,  1002,
    //     591,  550,  112,  378, 963,  335,  579,  715, 718,  843,  330, 472,  98,   606, 873, 931,
    //     127,  725,  254,  900, 927,  726,  27,   424, 767,  893,  262, 805,  645,  724, 964,  58,
    //     561,  709,  61,   435, 722,  483,  154,  619, 166,  149,  938, 181,  513,  213, 203, 496,
    //     503,  99,   474,  611, 831,  556,  835,  662, 11,   217,  514, 901,  1005, 940, 830, 515,
    //     7,    945,  705,  222, 150,  895,  682,  68,  507,  888,  969, 231,  134,  56,  147,  97,
    //     499,  553,  380,  327, 608,  363,  616,  743, 809,  844,  371, 557,  775,  569, 576, 732,
    //     252,  89,   620,  289, 535,  829,  277,  673, 868,  728,  54, 978,  864,  942, 439,  639,
    //     419,  892,  187,  658, 958,  793,  397,  308, 168,  259,  93, 521,  603,  840, 914,  334,
    //     183,  851,  950,  567, 962,  72,   704,  6,   118,  82,   559, 285,  303,  630, 721, 404,
    //     281,  278,  299,  362, 618,  296,  80,   142, 63,   288,  442, 968,  672,  101, 120, 577,
    //     545,  727,  509,  128, 16,   512,  91,   699, 452,  862,  887, 571,  1003, 53,  94, 907,
    //     137,  1020, 771,  711, 151,  628,  492,  126, 932,  686,  1014, 560,  377,  39,  678,
    //     741,  563,  415,  1006, 283, 531,  271,  200,  42,  588,  406,  237, 675,  768,  636,
    //     505,  746,  995,  47,   615,  518, 340,  223,  247,  364, 786,  774,  295, 977,  585,
    //     701, 393,  604,  387,  842,  882,  90,  762,  819,  527,  547, 450,  385,  870, 552, 947,
    //     596, 946,  903,  924,  980,  994,  102, 245,  114,  646,  849, 321,  582,  216, 24, 350,
    //     972, 919,  386,  508,  167,  302,  368, 652,  820,  280,  257, 1022, 955,  683, 326, 540,
    //     139, 331,  459,  235,  811,  409,  989, 374,  897,  838,  234, 510,  626,  951};

    uint16_t moves[] = {28, 6,  47, 43, 40, 12, 27, 7,  9,  30, 2,  5,  19, 61, 62, 51,
                        16, 59, 53, 45, 35, 3,  56, 52, 48, 49, 29, 46, 34, 14, 26, 0,
                        21, 23, 18, 55, 50, 22, 25, 38, 4,  33, 37, 63, 32, 31, 10, 17,
                        41, 24, 1,  8,  39, 36, 20, 60, 42, 44, 54, 15, 58, 57, 13, 11};

    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; i++) {
        HexState s = HexState(b);
        for (auto move : moves) {
            s.move(move);
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

    std::chrono::duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms";
}

int main() {
    HexBoard b = HexBoard();
    HexState s = HexState(b);

    HexState s2 = HexState(s);
    while (true) {
        s.print();
        uint16_t move = get_move_input(s);
        if (s.move(move)) {
            break;
        }
    }
    s.print();
    std::cout << (s.currentPlayer == Player::Black ? "Black" : "White") << " wins!" << std::endl;

    bench();
}
