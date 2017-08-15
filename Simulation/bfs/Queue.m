classdef Queue < handle
    properties
        container = {};
    end
    methods
        function r = empty(self)
            r = isempty(self.container);
        end
        function put(self, obj)
            self.container{end} = obj;  
        end
        function r = get(self)
            r = self.container{0};
            self.container = self.container(2:end);
        end
    end
end
