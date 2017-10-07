%% This is a shell that you will have to follow strictly. 
% You will use the plotmap() and viewmap() to display the outcome of your algorithm.

% Load sample_data_map_8, three variables will be created in your workspace. These were created as a 
% result of [m,v,s]=dfs('map_8.txt',[14,1],[1,18]);
% The solution can be viewed using 
% plotmap(m,s) 

% write your own function for the DFS algorithm.
function [retmap,retvisited,retsteps] = pathfinder( mapfile,startlocation,targetlocation)
    % Preprocessing
    start = mat2graphid(startlocation);
    target = mat2graphid(targetlocation);
    map = map_convert(mapfile);
    graph = generate_graph(map);
    
    % Processing  
    [came_from, steps] = search(graph, start, target);
    
    % Postprocessing
    retmap = map;
    
    keySet = came_from.keys();
    retvisited = ones(16, 16);
    for i = 1:length(keySet)
        visited_pos = graphid2mat(keySet{i});
        retvisited(visited_pos(1), visited_pos(2)) = 0;
    end
    
%     plotmap(map);
%     hold on;
%     for i = 1:length(steps)
%         visited_pos = graphid2mat(steps{i});
%         placestep([visited_pos(1), visited_pos(2)], i);
%     end
%     hold off;
    
    shortest = [];
    link = target;
    while(~isnan(link))
        shortest(end + 1, :) = graphid2mat(link); %#ok<AGROW>
        link = came_from(link);
    end
    retsteps = flipud(shortest);
end

function placestep(position,i)
% This function will plot a insert yellow rectangle and also print a number in this rectangle. Use with plotmap/viewmap. 
position = [16-position(1) position(2)];
position=[position(2)+0.1 position(1)+0.1];
rectangle('Position',[position,0.8,0.8],'FaceColor','y');
c=sprintf('%d',i);
text(position(1)+0.2,position(2)+0.2,c,'FontSize',10);
end
