function [ path, search_steps ] = graph_djs( graph, start, target )
%GRAPH_DJS Summary of this function goes here
%   Detailed explanation goes here
    max_queue_size = 0;
    frontier = PriorityQueue(floor(sqrt(length(graph.nodes))));
    frontier.put(0, start, 0);
    search_steps = start;
    cost_so_far = NaN(1, length(graph.nodes));
    cost_so_far(start) = 0;
    came_from = zeros(1, length(graph.nodes));
    came_from(start) = NaN;
    
    while ~frontier.empty()
        current = frontier.get();
        
        if current == target   
            break
        end
        
        edges = graph.edges{current};
        for i=1:length(edges)
            if edges{i}.a1(1) == current
                next = graph.nodes{edges{i}.a2(1)}.id;
            else
                next = graph.nodes{edges{i}.a1(1)}.id;
            end
            
            new_cost = cost_so_far(current) + 1; % Edge travel cost is here as '1'
            
            if isnan(cost_so_far(next)) || new_cost < cost_so_far(next)
                search_steps(end + 1) = next; %#ok<AGROW>
                cost_so_far(next) = new_cost;
                frontier.put(new_cost, next, 0);
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

