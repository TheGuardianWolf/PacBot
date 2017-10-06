%% This is a shell that you will have to follow strictly.
% You will use the plotmap() and viewmap() to display the outcome of your algorithm.

% Load sample_data_map_8, three variables will be created in your workspace. These were created as a
% result of [m,v,s]=dfs('map_8.txt',[14,1],[1,18]);
% The solution can be viewed using
% plotmap(m,s)

% write your own function for the DFS algorithm.
function [retmap,retvisited,retsteps] = dfs( mapfile,startlocation,targetlocation)
%A=mapfile(startlocation, targetlocation);
ClosedList{1,1} = startlocation;
while(ContinueLoop ==1)
    n = 1;
    counter = 0;
    if (counter == 0)
        Output = findNextPosition(n,CurrentPosition,targetlocation,ClosedList);
        counter = counter +1;
    else
        n= n + 1;
        counter = counter +1;
        if (NextPosition == targetlocation)
            ContinueLoop = 0;
        else
            CurrentPosition = NextPosition;
            Output = findNextPosition(n,CurrentPosition,targetlocation,ClosedList);
            NextPosition = Output(1,1);
            n = Ouput(1,2);
        end;
    end;
    
    disp(PossibleLocations);
    ContinueLoop = 0;
end
end



function placestep(position,i)
% This function will plot a insert yellow rectangle and also print a number in this rectangle. Use with plotmap/viewmap.
position = [16-position(1) position(2)];
position=[position(2)+0.1 position(1)+0.1];
rectangle('Position',[position,0.8,0.8],'FaceColor','y');
c=sprintf('%d',i);
text(position(1)+0.2,position(2)+0.2,c,'FontSize',10);
end
