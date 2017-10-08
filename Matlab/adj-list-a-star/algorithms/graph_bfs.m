function [ path, search_steps ] = graph_bfs( graph, start, target )
%GRAPH_BFS Summary of this function goes here
%   Detailed explanation goes here
    frontier = java.util.LinkedList;
    frontier.add(start);
    search_steps = start;
    came_from = zeros(1, length(graph.nodes));
    came_from(start) = NaN;
    
    while frontier.size() ~= 0
        current = frontier.remove();
        edges = graph.edges{current};
        
        if current == target   
            break
        end
        
        for i=1:length(edges)
            if edges{i}.a1(1) == current
                next = graph.nodes{edges{i}.a2(1)}.id;
            else
                next = graph.nodes{edges{i}.a1(1)}.id;
            end
            if came_from(next) == 0;
                search_steps(end + 1) = next; %#ok<AGROW>
                came_from(next) = current;
                frontier.add(next);
            end
        end
    end
    
    current = target;
    path = current;
    while path(end) ~= start
        current = came_from(current);
        path(end + 1) = current; %#ok<AGROW>
    end
    
    path = fliplr(path);
end

