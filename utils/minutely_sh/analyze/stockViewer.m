visualize_flag = 1;
visualize_movie_flag=1;
data =csvread('a');

timer = data(:, 1);
owarine = data(:, 2);

a_upper_v = data(:, 3);
a_lower_v = data(:, 5);
b_upper_v = data(:, 4);
b_lower_v = data(:, 6);
if (visualize_flag)
    f1 = figure();
    f2 = figure();
end
len = 30; 
for oi = 1:size(owarine)-len-1
    o = owarine(oi);
    %%%%%%%%%%%%%%%%%%%%%%%
    % plot slopes
    %%%%%%%%%%%%%%%%%%%%%%%
    if ((visualize_flag & visualize_movie_flag) | (visualize_flag & !visualize_movie_flag & oi == size(owarine, 1)))
        set(0,'CurrentFigure',f1);
        plot(timer, a_lower_v, 'b'); hold on; 
        plot(timer, a_upper_v, 'r'); 

        % plot stock price
        plot(timer, owarine, 'k');
        plot(timer(oi:oi+len), a_lower_v(oi)*[oi:oi+len]+b_lower_v(oi), 'b');

        xlim([0 380]); 
        ylim([min(owarine) max(owarine)]); 
        plot(timer(oi:oi+len), a_upper_v(oi)*[oi:oi+len]+b_upper_v(oi), 'r');
        hold off;

        set(0,'CurrentFigure',f2);
        hold on;
        xlim([0 380]); 
        plot(timer, a_lower_v, 'b');
        plot(timer, a_upper_v, 'r');
        plot(timer, a_upper_v-a_lower_v, 'g');
        hold off;
        if (visualize_movie_flag)
            pause(0.001);
        end
    end
end
pause;

