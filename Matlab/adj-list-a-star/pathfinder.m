%% This is a shell that you will have to follow strictly. 
% You will use the plotmap() and viewmap() to display the outcome of your algorithm.

% Load sample_data_map_8, three variables will be created in your workspace. These were created as a 
% result of [m,v,s]=pathfinder('map_8.txt',[14,1],[1,18]);
% The solution can be viewed using 
% plotmap(m,s) 

% write your own function for the DFS algorithm.
function [retmap,retvisited,retsteps] = pathfinder( mapfile,startlocation,targetlocation)
    % Preprocessing
    map = map_convert(mapfile);
    graph = Graph(map, 1024 * 2.5, 768 * 2.5);
    start = graph.grid2nodeid(startlocation(2), startlocation(1));
    target = graph.grid2nodeid(targetlocation(2), targetlocation(1));
    
    % Processing  
    [path, search_steps] = graph_bfs(graph, start, target);
    
    % Postprocessing
    retmap = map;
    retsteps = zeros(length(path), 2); 
    for i = 1:length(path)
        [v_x, v_y] = graph.nodeid2grid(path(i));
        retsteps(i, :) = [v_y, v_x];
    end
    retvisited = zeros(length(search_steps), 2);
    for i = 1:length(search_steps)
        [v_x, v_y] = graph.nodeid2grid(search_steps(i));
        retvisited(i, :) = [v_y, v_x];
    end
end