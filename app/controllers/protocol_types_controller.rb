class ProtocolTypesController < ApplicationController
  before_action :set_protocol_type, only: [:show, :edit, :update, :destroy]

  # GET /protocol_types
  # GET /protocol_types.json
  def index
    @protocol_types = ProtocolType.all
  end

  # GET /protocol_types/1
  # GET /protocol_types/1.json
  def show
  end

  # GET /protocol_types/new
  def new
    @protocol_type = ProtocolType.new
  end

  # GET /protocol_types/1/edit
  def edit
  end

  # POST /protocol_types
  # POST /protocol_types.json
  def create
    @protocol_type = ProtocolType.new(protocol_type_params)

    respond_to do |format|
      if @protocol_type.save
        format.html { redirect_to @protocol_type, notice: 'Protocol type was successfully created.' }
        format.json { render :show, status: :created, location: @protocol_type }
      else
        format.html { render :new }
        format.json { render json: @protocol_type.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /protocol_types/1
  # PATCH/PUT /protocol_types/1.json
  def update
    respond_to do |format|
      if @protocol_type.update(protocol_type_params)
        format.html { redirect_to @protocol_type, notice: 'Protocol type was successfully updated.' }
        format.json { render :show, status: :ok, location: @protocol_type }
      else
        format.html { render :edit }
        format.json { render json: @protocol_type.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /protocol_types/1
  # DELETE /protocol_types/1.json
  def destroy
    @protocol_type.destroy
    respond_to do |format|
      format.html { redirect_to protocol_types_url, notice: 'Protocol type was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_protocol_type
      @protocol_type = ProtocolType.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def protocol_type_params
      params.require(:protocol_type).permit(:protocolName)
    end
end
