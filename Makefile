.PHONY: clean All

All:
	@echo "----------Building project:[ Client - Debug ]----------"
	@cd "Client" && $(MAKE) -f  "Client.mk"
clean:
	@echo "----------Cleaning project:[ Client - Debug ]----------"
	@cd "Client" && $(MAKE) -f  "Client.mk" clean
