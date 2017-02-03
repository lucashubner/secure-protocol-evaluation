Rails.application.routes.draw do
  root 'sensor_reads#index'
  resources :statistics
  resources :protocol_types
  resources :sensor_reads
  resources :sensor_types
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
end
