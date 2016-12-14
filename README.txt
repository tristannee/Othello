Created by Tristan and Eshan

Over the past two weeks, work generally happened together, where we would both sit together using the same system, and switch back and forth on coding. 
While one person watched the other code, we would offer ideas back and forth, and if one person came up with an idea, he would go ahead and take over and code that up and see where the program went from there. 
Early on, Tristan did a lot of work in setting up the non minimax heuristic functionality by defining how we would run through arrays of spaces and check which one was a better space to move to then another, but Eshan helped a lot with implementing that into the actual AI and making it functional. 
In terms of doing a lot of optimizing, cleaning of the code, creating the minimax, adding comments, checking if stuff worked, etc., everything was almost evenly split when working together.

In order to make our AI tournament worthy, we took our non minimax function that looked at spaces, then made a minimax that looked at available moves and opponent moves, and scored our own best move based on the point weightage of the spaces we would end up with after the move and our opponents point weightage. 
For example, if we gained a lot of spots but the opponent gained a corner, that move would become a lot less favorable, while in a normal minimax, that move would be more favorable since the corner has no special weightage. 
Our strategy, once the spot weightage, will be the best because it choose spots in order of where is best to go relative to what the opponent has now and what they can potentially get, but the biggest problem we've been having is how to correctly weight the spots. 
Even slight adjustments of the weightage has a big impact on our ability to beat the higher level opponents, so that will take optimizing.
