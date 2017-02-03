require 'json'
class SensorReadsController < ApplicationController
  before_action :set_sensor_read, only: [:show, :edit, :update, :destroy]
  skip_before_action :verify_authenticity_token

# GET /sensor_reads
  # GET /sensor_reads.json
  def index
    @sensor_reads = SensorRead.all
  end

  # GET /sensor_reads/1
  # GET /sensor_reads/1.json
  def show
  end

  # GET /sensor_reads/new
  def new
    @sensor_read = SensorRead.new
  end

  # GET /sensor_reads/1/edit
  def edit
  end

  # POST /sensor_reads
  # POST /sensor_reads.json
  def create
    begin
     @sensor_read = SensorRead.new(sensor_read_params)
   rescue
      @payload_json = JSON.parse(self.env["rack.input"].as_json["input"][0].to_param)
      @sensor_read = SensorRead.new(@payload_json)
    end
    respond_to do |format|
      if @sensor_read.save
        format.html { redirect_to @sensor_read, notice: 'Sensor read was successfully created.' }
        format.any {render :show, status: :created, location: @sensor_read }
      else
        format.html { render :new }
        format.any {render json: @sensor_read.errors, status: :unprocessable_entity  }
      end
    end
  end

  # PATCH/PUT /sensor_reads/1
  # PATCH/PUT /sensor_reads/1.json
  def update
    respond_to do |format|
      if @sensor_read.update(sensor_read_params)
        format.html { redirect_to @sensor_read, notice: 'Sensor read was successfully updated.' }
        format.json { render :show, status: :ok, location: @sensor_read }
      else
        format.html { render :edit }
        format.json { render json: @sensor_read.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /sensor_reads/1
  # DELETE /sensor_reads/1.json
  def destroy
    @sensor_read.destroy
    respond_to do |format|
      format.html { redirect_to sensor_reads_url, notice: 'Sensor read was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_sensor_read
      @sensor_read = SensorRead.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def sensor_read_params
      params.require(:sensor_read).permit(:value, :sensor_type_id)
    end
end
