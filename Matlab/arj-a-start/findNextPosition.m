function [ NextPosition,n ] = findNextPosition(n, startlocation,targetlocation,ClosedList)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
map = map_convert('map_1.txt');
mapSize = (size(map));
% Horizontal_Cost = 10; Vertical_Cost = 10;
% Diagonal_Cost = 14;
% F_cost=0;
% G_cost=0;
% H_cost=0;
% ContinueLoop = 1;
ParentNode{1,n} = startlocation;
% Targetnode_Reached = 1;
for i=1:mapSize(1,1)
    for j = 1:mapSize(1,2)
        if map(i,j) == 1
            array = [i,j];
            Obstructions(i,j) = {array};
        else
            Obstructions(i,j) = {[0,0]};
            %disp(Current_obstruction);
        end;
    end;
end;

%ClosedSet(1,n) = startlocation;
PossibleLocations{3,3}(1,2) = 0; %values to be considered are stored in cell array
newPosition(1,2) =0;
% gcost is the cost to travel to the final destination node
%f cost is the some of the g and h costs
row = startlocation(1,1);
column = startlocation(1,2);

if(row == 1 && column == 1) %top left corner
    
elseif (row == mapSize(1,1)&& column == 1) %bottom left corner
    
elseif( (row == mapSize(1,1) && column == mapSize(1,2)))%bottom right corner
    
elseif((row == 1 && column ==mapSize(1,2)))%top right corner
    
elseif(row == 1) %any value in the first row
    
elseif(column == 1) %any value in the first colum
    
elseif(row == mapSize(1,1)) %any value in the last row
    
elseif (column == mapSize(1,2)) %any value in the last column
    
else
    for i=1:3
        if i ==1
            for x=1:3
                PossibleLocations{i,x} = startlocation(1,1)-1;
            end;
        elseif i == 2
            for x=1:3
                PossibleLocations{i,x} = startlocation(1,1);
            end;
        else
            for x=1:3
                PossibleLocations{i,x} = startlocation(1,1)+1;
            end;
        end;
    end;
end

for j=1:3
    if j ==1
        for x=1:3
            PossibleLocations{x,j}(1,2) = startlocation(1,2)-1;
        end
    elseif j == 2
        for x=1:3
            PossibleLocations{x,j}(1,2) = startlocation(1,2);
        end
    else
        for x=1:3
            PossibleLocations{x,j}(1,2) = startlocation(1,2)+1;
        end;
    end;
    
end;


for i = 1:mapSize(1,1)
    for j = 1:mapSize(1,2)
        valueToBeChecked = Obstructions{i,j};
        for x=1:3
            for y=1:3
                if(valueToBeChecked == PossibleLocations{x,y})
                    PossibleLocations{x,y} = 0; %if any obstruction value matched set the respective cell coordinates to 0
                end;
            end;
        end;
    end;
end;

for i=1:3
    for j=1:3
        %assigning the general and distance costs based on the positions of the
        %valid nodes
        if (PossibleLocations{i,j} ~= [0,0])
            %assigning distance costs using the manhattan method
            ChangeInX = abs(targetlocation(1,1)-PossibleLocations{i,j}(1,1)); %difference in the x 
            ChangeInY = abs(targetlocation(1,2)-PossibleLocations{i,j}(1,2)); % difference in the y
            %ConcatenatedResult = strcat(num2str(ChangeInX),num2str(ChangeInY)); %differences concatenated to form the number
            %DistanceCost = str2num(ChangeInX+ChangeInY);
            DistanceCost =ChangeInX+ChangeInY;
            PossibleLocations{i,j}(1,4) = DistanceCost;
            %assigning general costs
            if((i==1 && j==1) || (i==1 && j == 3) || (i==3 && j==1) || (i==3 && j ==3))
                PossibleLocations{i,j}(1,3)=14;
                PossibleLocations{i,j}(1,4)=PossibleLocations{i,j}(1,4)+14;
                
            elseif (i==2 && j==2)
                PossibleLocations{i,j}(1,3:4)=9999;%the parent node with 9999 value so that it can be avoided when finding
                                                   %the lowest value
            else
                PossibleLocations{i,j}(1,3)=10;
                PossibleLocations{i,j}(1,4)=PossibleLocations{i,j}(1,4)+10;
            end;
        else
            PossibleLocations{i,j}(1,3:4)=9999; %assign 0 to the rest of the costs
        end
        
    end
end

SmallestValueCoordinates=[];
SmallestValue = min(vertcat(PossibleLocations{[1 4]}));
%%anotherValue = min(vertcat(PossibleLocations{[2 4]}));
for i =1:3
    for j = 1:3
        
        if (PossibleLocations{i,j}(1,3:4)== SmallestValue(1,3:4)) %check if more than one value found, the value found will be in the chronological order
            SmallestValueCoordinates = PossibleLocations{i,j}(1,1:2);
        end
    end;
end;

NextPosition = SmallestValueCoordiantes;%after finidng the smallest value node add it to the closed list
SmallestValue(:,1:2) = []; %deleting the first two columns that contain values that are not important
valueFound = 0;


%% this function makes everyother node closed list
for i=1:3
    for j =1:3
        
        if (SmallestValue == PossibleLocations{i,j}(3:4))
            a = size(ClosedList);
            for x=1:a(1,2)
                if(SmallestValue ~= ClosedList(1,x))
                   ValueDetectedInClosedLoop = 1;
                end;
            end;
           if (ValueDetectedInClosedLoop == 0)
                   valueFound = PossibleLocations{i,j}(1:2);
           else
                PossibleLocations{i,j}(1,3:4)=9999; %if the node has already been checked then set its cost high
                                                    %to avoid any further
                                                    %checks
           end; 
           end
        end
    end


NextPosition = valueFound;
Closedlist(:) = valueFound;
n=n+1
%ClosedSet(1,n) = NextPosition;
%SmallestValue = PossibleLocations{:}(1,4);
%[M,I] = min(SmallestValue);


%if(startlocation(1,1) >= mapSize(1,2))     %if startlocation column is greater than or equal to the number of columns

%elseif (startlocation(1,1) <= mapSize(1,1))


%end;
%calculate the node values cost
%calculate distance from the start node to the end node
%check for cases where the start and the end values are the same.

%need to calculate the g value for every possible node
end


