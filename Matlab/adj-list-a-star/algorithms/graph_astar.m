function [ path, search_steps ] = graph_astar( graph, start, target )
%GRAPH_DJS Summary of this function goes here
%   Detailed explanation goes here
    max_queue_size = 0;
    frontier = PriorityQueue(floor(length(graph.nodes) / 2));
    frontier.put(0, start, 0);
    search_steps = start;
    cost_so_far = NaN(1, length(graph.nodes));
    cost_so_far(start) = 0;
    came_from = zeros(1, length(graph.nodes));
    came_from(start) = NaN;
    
    while ~frontier.empty()
        current = frontier.get();
        
%         if current == target   
%             break
%         end
        
        edges = graph.edges{current};
        for i=1:length(edges)
            [next, ~] = edges{i}.get_arc_from(current);
            
            new_cost = cost_so_far(current) + 1; % Edge travel cost is here as '1'
            
            if next == target   
                search_steps(end + 1) = next; %#ok<AGROW>
                cost_so_far(next) = new_cost;
                came_from(next) = current;
                frontier.size = 0;
                break
            end
            
            if isnan(cost_so_far(next)) || new_cost < cost_so_far(next)
                search_steps(end + 1) = next; %#ok<AGROW>
                cost_so_far(next) = new_cost;
                heuristics_cost = heuristic(graph.nodes{next}.pos_grid, graph.nodes{target}.pos_grid);
                frontier.put(new_cost + heuristics_cost, next, 0);
                came_from(next) = current;
            end
            
            if frontier.size > max_queue_size
                max_queue_size = frontier.size;
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

function cost = heuristic(node_grid_pos, target_grid_pos)
    dx = abs(node_grid_pos(1) - target_grid_pos(1));
    dy = abs(node_grid_pos(2) - target_grid_pos(2));
    cost = dx + dy;
end

