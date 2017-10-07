function [ path, search_steps ] = graph_bfs( graph, start, target )
%GRAPH_BFS Summary of this function goes here
%   Detailed explanation goes here
    frontier = java.util.LinkedList;
    frontier.add(start);
    search_steps = start;
    came_from = zeros(1, length(graph.nodes));
    
    while frontier.size() ~= 0
        current = frontier.removeLast();
        edges = graph.edges{current};
        
        if current.id == target   
            break
        end
        
        for i=1:length(edges)
            if edges{i}.a1(0) == current.id
                next = graph.nodes{edges{i}.a2(0)};
            else
                next = graph.nodes{edges{i}.a1(0)};
            end
            if came_from(next) == 0;
                search_steps(end + 1) = next; %#ok<AGROW>
                came_from(next) = current.id;
                frontier.add(next);
            end
        end
    end
    
    path = current.id;
    current_id = current.id;
    while came_from(current_id) ~= 0
        current_id = came_from(current_id);
        path(end + 1) = current_id; %#ok<AGROW>
    end
    
    path = fliplr(path);
end

