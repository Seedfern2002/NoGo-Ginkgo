## NOGO bot: The course project of *Introduction to Computation(A)* 2020 Fall in PKU
This is the course project of *Introduction to Computation(A)* 2020 Fall made by Yexiang Cheng in his freshman year. The bot is based on alpha-beta pruning, you can play with it in [Botzone](https://botzone.org.cn/), or run [play.cpp](play.cpp) which provides a simple board and easy mode bot.

#### 1. NoGo
This game has similar rules to Go, with the only difference being that the player who eats the opponent's pieces or is unable to put new piece loses the game. 
#### 2. File description
- [Ginkgo.cpp](Ginkgo.cpp): The source code of Ginkgo on the [ranking list](https://botzone.org.cn/game/ranklist/5ab65ae77ec1de5c52e18940?page=0#5fe311b6d9383f7579a47911).
- [jsoncpp.cpp](jsoncpp.cpp): The json interface with [Botzone](https://botzone.org.cn/). 
- [play.cpp](play.cpp): Natively run program in which you can play NoGo with a watered-down version of Ginkgo. 